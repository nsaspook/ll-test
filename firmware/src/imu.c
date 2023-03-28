#include "imupic32mcj.h"
#include "imu.h"

const double imu_table[] = {
	BMA490_ACCEL_MG_LSB_2G,
	BMA490_ACCEL_MG_LSB_4G,
	BMA490_ACCEL_MG_LSB_8G,
	BMA490_ACCEL_MG_LSB_16G,
	IMU_ACCEL_MG_SCALE,
	SCA3300_ACCEL_MG_LSB_15G,
	SCA3300_ACCEL_MG_LSB_3G,
	SCA3300_ACCEL_MG_LSB_6G,
	SCL3300_ACCEL_MG_LSB_12G,
	SCL3300_ACCEL_MG_LSB_24G,
	SCL3300_INC1,
	SCL3300_INC2,
};

static const uint8_t imu_name [][8] = {
	"BMA4x0",
	"SCA3300",
	"SCL3300",
	"BMA400",
	"NO IMU ",
};

char imu_buffer[256];
static uint32_t sensortime;

static void move_bma490_transfer_data(uint8_t *, imu_cmd_t *);

extern struct sca3300_data sdata;

double get_imu_scale(imu_cmd_t * imu)
{
	double accelRange = IMU_ACCEL_MG_SCALE;

	if (imu) { // null pointer check
		/*
		 * load the proper scaling constants
		 */
		switch (imu->acc_range) {
		case range_16g:
			accelRange = BMA490_ACCEL_MG_LSB_16G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_8g:
			accelRange = BMA490_ACCEL_MG_LSB_8G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_4g:
			accelRange = BMA490_ACCEL_MG_LSB_4G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_15g:
		case range_15gl:
			accelRange = SCA3300_ACCEL_MG_LSB_15G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_3g:
			accelRange = SCA3300_ACCEL_MG_LSB_3G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_6g:
			accelRange = SCA3300_ACCEL_MG_LSB_6G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_12g:
			accelRange = SCL3300_ACCEL_MG_LSB_12G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_24g:
			accelRange = SCL3300_ACCEL_MG_LSB_24G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_inc1:
			accelRange = SCL3300_INC1 * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
		case range_inc2:
			accelRange = SCL3300_INC2 * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
#ifdef BMA400
		case range_2g:
			accelRange = BMA400_ACCEL_MG_LSB_2G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
#else
		case range_2g:
			accelRange = BMA490_ACCEL_MG_LSB_2G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
			break;
#endif
		default:
			if (imu->device == IMU_BMA490L) {
				accelRange = BMA400_ACCEL_MG_LSB_2G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
				imu->acc_range = range_2g; // update imu data structure
			} else {
				if (imu->device == IMU_SCA3300) {
					accelRange = SCA3300_ACCEL_MG_LSB_15G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
					imu->acc_range = range_15g; // update imu data structure
				} else {
					accelRange = SCL3300_ACCEL_MG_LSB_12G * GRAVITY_EARTH * IMU_ACCEL_MG_SCALE;
					imu->acc_range = range_12g; // update imu data structure
				}
			}
			break;
		}
	}

	return accelRange;
}

void getAllData(sSensorData_t *accel, imu_cmd_t * imu)
{
	uint8_t data[IMU_DATA_RAW_LEN + 2] = {0}; // add space for dummy data
	int16_t x = 0, y = 0, z = 0;
#ifdef SCA3300
	int16_t xa = 0, ya = 0, za = 0;
#endif
	double accelRange;

	accelRange = get_imu_scale(imu);

	// munge data to proper format for logging
	if (imu) { // null pointer check
		if (accel) { // null pointer check
			if (imu->device == IMU_BMA490L) {
#ifdef BMA400
				/*
				 * need to swap X-Y and reverse sign to match SCA3000
				 * Z is fine
				 */
				uint16_t lsb;
				uint8_t msb;

				move_bma490_transfer_data(data, imu);
				sensortime = (data[9] << 16) | (data[8] << 8) | data[7]; // 24-bit sensor time
				sdata.scan.ts = sensortime;

				lsb = data[3];
				msb = data[4];
				x = (int16_t) (((uint16_t) msb * 256) + lsb); // 12-bit xyz data
				if (x > 2047) {
					/* Computing accel data negative value */
					x = x - 4096;
				}
				x = -x; // invert sign, no bit-twiddling
				sdata.scan.channels[SCA3300_ACC_X] = x;

				lsb = data[1];
				msb = data[2];
				y = (int16_t) (((uint16_t) msb * 256) + lsb);
				if (y > 2047) {
					/* Computing accel data negative value */
					y = y - 4096;
				}
				y = -y; // invert sign
				sdata.scan.channels[SCA3300_ACC_Y] = y;

				lsb = data[5];
				msb = data[6];
				z = (int16_t) (((uint16_t) msb * 256) + lsb);
				if (z > 2047) {
					/* Computing accel data negative value */
					z = z - 4096;
				}
				sdata.scan.channels[SCA3300_ACC_Z] = z;
				accel->sensortime = sensortime; // time log each accel measurement from IMU		
#else
				move_bma490_transfer_data(data, imu);
				sensortime = (data[9] << 16) | (data[8] << 8) | data[7]; // 24-bit sensor time
				sdata.scan.ts = sensortime;
				x = (int16_t) (((uint16_t) data[2] << 8) | data[1]); // 16-bit xyz data
				sdata.scan.channels[SCA3300_ACC_X] = x;
				y = (int16_t) (((uint16_t) data[4] << 8) | data[3]);
				sdata.scan.channels[SCA3300_ACC_Y] = y;
				z = (int16_t) (((uint16_t) data[6] << 8) | data[5]);
				sdata.scan.channels[SCA3300_ACC_Z] = z;
				accel->sensortime = sensortime; // time log each accel measurement from IMU
#endif
			}
#ifdef SCA3300
			x = sdata.scan.channels[SCA3300_ACC_X];
			y = sdata.scan.channels[SCA3300_ACC_Y];
			z = sdata.scan.channels[SCA3300_ACC_Z];
			accel->sensortime = sdata.scan.ts; // time log each accel measurement from TIMER
			accel->sensortemp = TEMPERATURE_OFFSET + (sdata.scan.channels[SCA3300_TEMP] / TEMPERATURE_RES);
			if (imu->device == IMU_SCL3300) {
				xa = sdata.scan.channels[SCL3300_ANG_X];
				ya = sdata.scan.channels[SCL3300_ANG_Y];
				za = sdata.scan.channels[SCL3300_ANG_Z];
				accel->xa = xa / ANGLE_RES1 * ANGLE_RES2; // scale angle data
				accel->ya = ya / ANGLE_RES1 * ANGLE_RES2;
				accel->za = za / ANGLE_RES1 * ANGLE_RES2;
			}
#endif
			accel->x = x * accelRange; // scale to the correct units
			accel->y = y * accelRange;
			accel->z = z * accelRange;
		}
	}
}

/*
 * load raw SPI sensor data from IMU and transfer to the logging processing buffer
 */
void move_bma490_transfer_data(uint8_t *pBuf, imu_cmd_t * imu)
{
	if (pBuf && imu) { // null pointer checks
		for (uint32_t i = IMU_DATA_BUFFER_INDEX; i < IMU_DATA_RAW_LEN; i++) {
			pBuf[i - IMU_DATA_BUFFER_INDEX] = imu->rbuf[i];
		}
	}
}

const uint8_t * imu_string(imu_cmd_t * imu)
{
	const uint8_t * str_ptr = imu_name[IMU_NONE];

	if (imu) { // null pointer check
		if (imu->device < IMU_LAST) {
			str_ptr = imu_name[imu->device];
		}
	}
	return str_ptr; // return none string

}
