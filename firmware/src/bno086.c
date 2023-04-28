#include "bno086.h"

// sizes of various sensor data packet elements
#define SIZEOF_BASE_TIMESTAMP 5
#define SIZEOF_TIMESTAMP_REBASE 5
#define SIZEOF_ACCELEROMETER 10
#define SIZEOF_LINEAR_ACCELERATION 10
#define SIZEOF_GYROSCOPE_CALIBRATED 10
#define SIZEOF_MAGNETIC_FIELD_CALIBRATED 10
#define SIZEOF_MAGNETIC_FIELD_UNCALIBRATED 16
#define SIZEOF_ROTATION_VECTOR 14
#define SIZEOF_GAME_ROTATION_VECTOR 12
#define SIZEOF_GEOMAGNETIC_ROTATION_VECTOR 14
#define SIZEOF_TAP_DETECTOR 5
#define SIZEOF_STABILITY_REPORT 6
#define SIZEOF_STEP_DETECTOR 8
#define SIZEOF_STEP_COUNTER 12
#define SIZEOF_SIGNIFICANT_MOTION 6
#define SIZEOF_SHAKE_DETECTOR 6

static const char *build_date = __DATE__, *build_time = __TIME__;

sh2_Quaternion_t fusion_q;

uint8_t * txShtpHeader = imu0.tbuf;
uint8_t * rxShtpHeader = imu0.rbuf;

uint8_t * txShtpData = imu0.tbuf + SHTP_HEADER_SIZE;
uint8_t * rxShtpData = imu0.rbuf + SHTP_HEADER_SIZE;

uint16_t rxPacketLength;

/// Current sequence number for each channel, incremented after transmission. 
uint8_t sequenceNumber[6];

/// Commands have a seqNum as well. These are inside command packet, the header uses its own seqNum per channel
uint8_t commandSequenceNumber;

void bno086_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	static bool first = true;
	bool wait = true;

	// set SPI MODE
	set_imu_bits(); // set 8 or 32-bit SPI transfers
	LED_GREEN_Off();
	LED_RED_Off();
	if (imu) {
		if (first) { // power up restart device detection mode
			imu->update = false;
		}
		/*
		 * trigger ISR on IMU data update interrupts
		 */
		init_imu_int_bno(imu);

		if (first) { // lets see if the device is alive and talking
			first = false;
			StartTimer(TMR_BNO_INT, BNO_INIT_DELAY_INT);
			while (wait) {
				if (TimerDone(TMR_BNO_INT)) { // timeout waiting for device
					wait = false; // exit wait loop to fail detection
					LED_RED_On();
					snprintf(imu_buffer, max_buf, "** BNO08X detection failed **");
					imu->init_good = false;
				}
				if (imu->update) { // ISR set detection flag
					wait = false;
					LED_GREEN_On();
					snprintf(imu_buffer, max_buf, "BNO08X interrupt detected");

					/*
					 * let's talk to the device
					 */
					bno086_get_header(imu); // first 4 bytes

					if (bno086_get_cpacket(SHTP_HEADER_SIZE, imu)) {
						imu->init_good = true;
					} else { // bad or no packet
						imu->init_good = false;
						return;
					}
				}
			}
		}

	}
}

bool sendPacket(uint8_t channelNumber, uint8_t dataLength, void * imup)
{
	imu_cmd_t * imu = imup;
	uint16_t totalLength = dataLength + 4; //Add four bytes for the header

	imu->tbuf[0] = totalLength & 0xFF;
	imu->tbuf[1] = totalLength >> 8;
	imu->tbuf[2] = channelNumber;
	imu->tbuf[3] = sequenceNumber[channelNumber];

	// wipe out any existing RX packet header so we know if we received a packet
	memset(imu->rbuf, 0, SHTP_HEADER_SIZE);

	// send packet to IMU.
	// This also might receive the first part of another packet, which there is no way to avoid.
	SPI2_WriteRead(imu->tbuf, totalLength, imu->rbuf, totalLength);

	if (imu->rbuf[0] == 0 && imu->rbuf[0] == 0) {
		// no header data so no packet received
		return true;
	} else {
		// received first part of data packet while writing
		if (bno086_get_cpacket(totalLength, imu)) {
			// received data packet, send to proper channels
			processPacket();
			return true;
		}

		// receive failed
		return false;
	}
}

void processPacket(void)
{

	if (rxShtpHeader[2] == CHANNEL_CONTROL) {
		// currently no command reports are read
	} else if (rxShtpHeader[2] == CHANNEL_EXECUTABLE) {
		// currently no executable reports are read
	} else if (rxShtpHeader[2] == CHANNEL_COMMAND) {

	} else if (rxShtpHeader[2] == CHANNEL_REPORTS || rxShtpHeader[2] == CHANNEL_WAKE_REPORTS) {
		if (rxShtpData[0] == SHTP_REPORT_BASE_TIMESTAMP) {
			// sensor data packet
			parseSensorDataPacket();
		}
	}
}

void parseSensorDataPacket(void)
{
	
}

void bno086_get_header(void * imup)
{
	imu_cmd_t * imu = imup;

	SPI2_WriteRead(NULL, 0, imu->rbuf, SHTP_HEADER_SIZE);
}

// check for new data and read contents into imu.rbuf

bool bno086_receive_packet(void * imup)
{
	imu_cmd_t * imu = imup;

	bno086_get_header(imu); // first 4 bytes
	return bno086_get_cpacket(SHTP_HEADER_SIZE, imu);
}

bool bno086_get_cpacket(size_t read_b, void * imup)
{
	imu_cmd_t * imu = imup;
	uint16_t totalLength;

	if (imu->rbuf[0] == 0xff && imu->rbuf[1] == 0xff) { // check for invalid device data
		LED_RED_On(); // 
		return false;
	}

	//Calculate the number of data bytes in this packet
	totalLength = ((uint16_t) (imu->rbuf[1]) << 8 | imu->rbuf[0]);

	// Clear the MSbit.
	totalLength &= ~(1 << 15);

	if (totalLength == 0) {
		// Packet is empty
		return(false); //All done
	}

	rxPacketLength = totalLength - SHTP_HEADER_SIZE;

	if (totalLength <= read_b) {
		// the original transaction already read the completed packet!  We're done.
		return true;
	}

	if (rxPacketLength > SHTP_RX_PACKET_SIZE) {
		return false;
	}

	if (read_b == SHTP_HEADER_SIZE) {
		// just read the entire packet into the buffer
		SPI2_WriteRead(NULL, 0, imu->rbuf, totalLength);
	} else {
		// we want to receive a new header, plus the remaining data bytes that haven't been read.
		size_t receiveLength = SHTP_HEADER_SIZE + (totalLength - read_b);

		// read remaining bytes into the data buffer starting at the next byte
		SPI2_WriteRead(NULL, 0, imu->rbuf + read_b, receiveLength);

		// erase the new header we just read, leaving only the data as a contiguous block
		memmove(imu->rbuf + read_b, imu->rbuf + read_b + SHTP_HEADER_SIZE, receiveLength - SHTP_HEADER_SIZE);
	}

	return true;
}

bool bno086_getid(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
		}
		return imu->online;
	} else {
		return false;
	}
}

bool bno086_getdata(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
		}
		return imu->online;
	} else {
		return false;
	}
}

void bno086_version(void)
{
	snprintf(imu_buffer, max_buf, "%s Driver Version  %s %s %s ", BNO086_ALIAS, BNO086_DRIVER, build_date, build_time);
}
