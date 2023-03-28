#include "sca3300.h"

static const char *build_date = __DATE__, *build_time = __TIME__;

static uint8_t CRC8(uint8_t, uint8_t);

static bool imu_cs(imu_cmd_t *);
static void imu_cs_cb(uintptr_t);

void sca3300_cs_disable(imu_cmd_t *);

/*
 * data from IMU query
 */
struct sca3300_data sdata = {
	.id = CAN_IMU_RAW,
};

// Calculate CRC for 24 MSB's of the 32 bit dword
// (8 LSB's are the CRC field and are not included in CRC calculation)

uint8_t CalculateCRC(uint32_t Data)
{
	uint8_t BitIndex;
	uint8_t BitValue;
	uint8_t CRC;

	CRC = 0xFF;
	for (BitIndex = 31; BitIndex > 7; BitIndex--) {
		BitValue = (uint8_t) ((Data >> BitIndex) & 0x01);
		CRC = CRC8(BitValue, CRC);
	}
	CRC = (uint8_t) ~CRC;
	return CRC;
}

static uint8_t CRC8(uint8_t BitValue, uint8_t CRC)
{
	uint8_t Temp;

	Temp = (uint8_t) (CRC & 0x80);
	if (BitValue == 0x01) {
		Temp ^= 0x80;
	}
	CRC <<= 1;
	if (Temp > 0) {
		CRC ^= 0x1D;
	}
	return CRC;
}

/*
 * check for a proper 8-bit CRC in bits [0..7] of the 32-bit word selected by option
 */
bool sca3300_check_crc(imu_cmd_t * imu, uint8_t option)
{
	bool crc = true;

	switch (option) {
	case SCA3300_REC:
	default:
		if (CalculateCRC(imu->rbuf32[SCA3300_REC]) != (imu->rbuf32[SCA3300_REC]&0xff)) {
			imu->crc_error = true; // set of CRC failure of returned data
			crc = false;
		}
		break;
	}
	return crc;
}

/*
 * talk to the IMU via the SPI port
 */
bool sca3300_imu_transfer(imu_cmd_t * imu, uint32_t data)
{
	imu_cs(imu); // select IMU on SPI bus after required delay between SPI requests
	imu->tbuf32[SCA3300_TRM] = data; // data request command as 32-bit word with CRC
	SPI2_WriteRead(imu->tbuf32, SCA3300_CHIP_BTYES_PER_SPI, imu->rbuf32, SCA3300_CHIP_BTYES_PER_SPI);
	StartTimer(TMR_CS, SCA3300_CHIP_CS_DELAY); // milliseconds
	while (imu->run) { // wait until data has left the SPI buffer, run flag is set in SPI interrupt ISR
		if (TimerDone(TMR_CS)) {
			return false;
		}
	};

	return true;
}

/*
 * Read raw ACCEL data from the chip using SPI
 * check all returned SPI data for proper CRC
 * Off-frame protocol is used so each
 * transfer consists of two phases. A response to the request is sent within next request
 * frame. The response concurrent to the request contains the data requested by the
 * previous command.
 * ~160us runtime per full data update
 */
bool sca3300_getdata(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
			// junk first response
			imu->crc_error = false; // reset CRC checking flag
			// dummy result return on first command

			sca3300_imu_transfer(imu, SCA3300_ACC_X_32B);
			sca3300_check_crc(imu, SCA3300_REC); // check dummy for CRC error

			sca3300_imu_transfer(imu, SCA3300_ACC_Y_32B);
			if (sca3300_check_crc(imu, SCA3300_REC)) {
				sdata.scan.channels[SCA3300_ACC_X] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // X data
			};

			sca3300_imu_transfer(imu, SCA3300_ACC_Z_32B);
			if (sca3300_check_crc(imu, SCA3300_REC)) {
				sdata.scan.channels[SCA3300_ACC_Y] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // Y data
			};

			sca3300_imu_transfer(imu, SCA3300_TEMP_32B);
			if (sca3300_check_crc(imu, SCA3300_REC)) {
				sdata.scan.channels[SCA3300_ACC_Z] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // Z data
			};

			sca3300_imu_transfer(imu, SCA3300_RS_32B);
			if (sca3300_check_crc(imu, SCA3300_REC)) {
				sdata.scan.channels[SCA3300_TEMP] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // Z data
			};

			sca3300_imu_transfer(imu, SCA3300_RS_32B);
			if (sca3300_check_crc(imu, SCA3300_REC)) {
				sdata.scan.ret_status = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // return status data
			};

			if (imu->device == IMU_SCL3300) { // we also need angles data
				sca3300_imu_transfer(imu, SCL3300_ANG_X_32B);
				sca3300_check_crc(imu, SCA3300_REC); // check dummy for CRC error

				sca3300_imu_transfer(imu, SCL3300_ANG_Y_32B);
				if (sca3300_check_crc(imu, SCA3300_REC)) {
					sdata.scan.channels[SCL3300_ANG_X] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // X data
				};

				sca3300_imu_transfer(imu, SCL3300_ANG_Z_32B);
				if (sca3300_check_crc(imu, SCA3300_REC)) {
					sdata.scan.channels[SCL3300_ANG_Y] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // Y data
				};

				sca3300_imu_transfer(imu, SCA3300_RS_32B);
				if (sca3300_check_crc(imu, SCA3300_REC)) {
					sdata.scan.channels[SCL3300_ANG_Z] = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff); // Z data
				};
			}

#ifdef __32MK0512MCJ048__
			sdata.scan.ts = TMR9_CounterGet(); // load a clock time-stamp from timer9 32-bit counter, frequency 234,375KHz, 266.66 min roll-over
#endif
#ifdef __32MZ1025W104132__
			sdata.scan.ts = TMR2_CounterGet(); // load a clock time-stamp from timer9 32-bit counter, frequency 234,375KHz, 266.66 min roll-over
#endif
		}
		return imu->online;
	} else {
		return false;
	}
}

/*
 * see if we can get the correct ID response in rbuf32
 */
bool sca3300_getid(void * imup)
{
	imu_cmd_t * imu = imup;
	bool angles = false;

	if (imu) {
		if (!imu->run) {
			delay_us(SCA3300_CHIP_ID_DELAY); // sca3300 ID command spacing
			sca3300_imu_transfer(imu, SCA3300_WHOAMI_32B);
			if ((((imu->rbuf32[SCA3300_REC] >> 8)&0xffff) == SCA3300_WHOAMI_ID) || (angles = ((imu->rbuf32[SCA3300_REC] >> 8)&0xffff) == SCA3300_WHOAMI_ID_SCL)) {
				if (sca3300_check_crc(imu, SCA3300_REC)) {
					if (angles) { // SCL3300 detected
						imu->angles = true; // SLC3300 mode
						imu->device = IMU_SCL3300;
						imu->acc_range = imu->acc_range_scl; // set to SCL ranges
						sca3300_imu_transfer(imu, SCL3300_ANGLE); // enable angle data
						delay_us(SCA3300_CHIP_MODE_DELAY);
					}
					imu->online = true;
					imu->rbuf32[SCA3300_REC] = 0;
					imu->crc_error = false;
					sca3300_getserial(imu);
				} else {
					imu->crc_error = true;
				}
			} else {
				imu->online = false;
			}
		}
		return imu->online;
	} else {
		return false;
	}
}

/*
 * get IMU device serial number and convert that to 29-bit ID for CAN-FD
 */
bool sca3300_getserial(void * imup)
{
	imu_cmd_t * imu = imup;
	bool gotserial = false;

	if (imu) {
		if (!imu->run) {
			delay_us(SCA3300_CHIP_ID_DELAY); // sca3300 command spacing
			sca3300_imu_transfer(imu, SCA3300_BANK1);
			sca3300_imu_transfer(imu, SCA3300_SERIAL1);
			sca3300_imu_transfer(imu, SCA3300_SERIAL2);
			imu->serial1 = (imu->rbuf32[SCA3300_REC] >> 8)&0xffff;
			sca3300_imu_transfer(imu, SCA3300_BANK0);
			if (sca3300_check_crc(imu, SCA3300_REC)) {
				imu->serial2 = (imu->rbuf32[SCA3300_REC] >> 8)&0xffff;
				gotserial = true;
				board_serial_id = (imu->serial1 + (imu->serial2 << 16)) &0x1fffffff; // reduce to 29-bit id for CAN-CD address
				imu->board_serial_id = board_serial_id;
			}
		}
	}
	return gotserial;
}

/*
 * check SPI and device setup
 */
void sca3300_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	enum accel_g accel_range = SCA3300_MODE1;

	set_imu_bits(); // set 8 or 32-bit SPI transfers
	LED_GREEN_Off();
	LED_RED_On();
	if (imu) {
		sca3300_getid(imu);
		sca3300_getid(imu);
		sca3300_imu_transfer(imu, SCA3300_SWRESET_32B); // chip software reset
		delay_us(SCA3300_CHIP_SWR_DELAY);
		switch (imu->acc_range) { // set the range variable
		case range_15g:
			accel_range = SCA3300_MODE3; // set to 1.5g full-scale, 70 Hz 1st order low pass filter
			break;
		case range_15gl:
			accel_range = SCA3300_MODE4; // set to 1.5g full-scale, 10 Hz 1st order low pass filter
			break;
		case range_6g:
			accel_range = SCA3300_MODE2; // set to 6g full-scale, 70 Hz 1st order low pass filter
			break;
		case range_3g:
			accel_range = SCA3300_MODE1; // set to 3g full-scale, 70 Hz 1st order low pass filter
			break;
		case range_12g:
			accel_range = SCL3300_MODE1; // set to 1.2g full-scale, 40 Hz 1st order low pass filter
			break;
		case range_24g:
			accel_range = SCL3300_MODE2; // set to 2.4g full-scale, 70 Hz 1st order low pass filter
			break;
		case range_inc1:
			accel_range = SCL3300_MODE3; // Inclination mode, 10 Hz 1st order low pass filter
			break;
		case range_inc2:
			accel_range = SCL3300_MODE4; // Inclination mode low noise, 10 Hz 1st order low pass filter
			break;
		default:
			if (imu->device == IMU_SCA3300) {
				accel_range = SCA3300_MODE1; // set to 3g full-scale, 70 Hz 1st order low pass filter
			} else {
				accel_range = SCL3300_MODE1; // set to 1.2g full-scale, 40 Hz 1st order low pass filter
			}
			break;
		}
		sca3300_imu_transfer(imu, accel_range); // send the range command
		delay_us(SCA3300_CHIP_MODE_DELAY);
		sca3300_imu_transfer(imu, SCA3300_RS_32B);
		sca3300_imu_transfer(imu, SCA3300_RS_32B);
		sca3300_imu_transfer(imu, SCA3300_RS_32B);
		sca3300_imu_transfer(imu, SCA3300_RS_32B);
		imu->ss = (imu->rbuf32[SCA3300_REC] >> 8)&0xfff;
		imu->rs = (imu->rbuf32[SCA3300_REC] >> 24)&0x3;
	}
}

/*
 * enable sca3300 CS and set run flag that will be cleared when the buffer interrupt
 * happens or the disable function is call manually
 * we only use one gpio CS line on this board for either type of IMU device
 */
bool imu_cs(imu_cmd_t * imu)
{
	if (imu) {
		switch (imu->cs) {
		case 0:
		default:
			delay_us(SCA3300_CHIP_CS_DELAY); // 
			imu->run = true;
			IMU_CS_Clear();
			// set SPI receive complete callback
			SPI2_CallbackRegister(imu_cs_cb, (uintptr_t) imu);
			break;
		}
		return true;
	} else {
		return false;
	}
}

/*
 * force sca3300 CS disabled and clear run flag
 */
void sca3300_cs_disable(imu_cmd_t * imu)
{
	if (imu) {
		switch (imu->cs) {
		case 0:
		default:
			imu->run = false;
			IMU_CS_Set();
			break;
		}
	}
}

/*
 * SPI interrupt completed callback
 * disables sca3300 CS and clears run flags
 */
void imu_cs_cb(uintptr_t context)
{
	imu_cmd_t * imu = (void*) context;

	if (imu) {
		switch (imu->cs) {
		case 0:
		default:
			IMU_CS_Set();
			imu->run = false;
			break;
		}
	}
}

void sca3300_version(void)
{
	snprintf(imu_buffer, max_buf, " %s Driver Version  %s %s %s", SCA3300_ALIAS, SCA3300_DRIVER, build_date, build_time);
}

