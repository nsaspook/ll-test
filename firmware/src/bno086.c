#include "bno086.h"
#include "quat.h"

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

#define STATUS_ARRAY_LEN MAX_SENSOR_REPORTID + 1

/// stores status of each sensor, indexed by report ID
uint8_t reportStatus[STATUS_ARRAY_LEN];

/// stores whether a sensor has been updated since the last call to hasNewData()
bool reportHasBeenUpdated[STATUS_ARRAY_LEN];

Quaternion rotationVector, totalAcceleration, linearAcceleration, gravityAcceleration, gyroRotation, magField,
magFieldUncalibrated, hardIronOffset, gameRotationVector, geomagneticRotationVector;

bool tapDetected;
bool doubleTap;

enum Stability stability;
bool stepDetected;
uint16_t stepCount;
bool significantMotionDetected;
bool shakeDetected;
bool xAxisShake;
bool yAxisShake;
bool zAxisShake;

uint8_t majorSoftwareVersion;
uint8_t minorSoftwareVersion;
uint16_t patchSoftwareVersion;
uint32_t partNumber;
uint32_t buildNumber;

/**
 * Auxiliary accuracy readout from the Rotation Vector report.
 * Represents the estimated accuracy of the rotation vector in radians.
 */
float rotationAccuracy, geomagneticRotationAccuracy;

void clearSendBuffer(void * imup)
{
	imu_cmd_t * imu = imup;

	memset(imu->tbuf, 0, SHTP_HEADER_SIZE + SHTP_MAX_TX_PACKET_SIZE);
}

void bno086_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	static bool first = true;
	bool wait = true;

	IMU_CS_Set();
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
		snprintf(cmd_buffer, max_buf, "init_imu_int_bno");
		WaitMs(20);

		if (first) { // lets see if the device is alive and talking
			first = false;
			StartTimer(TMR_BNO_INT, BNO_INIT_DELAY_INT);
			while (wait) {
				if (TimerDone(TMR_BNO_INT)) { // timeout waiting for device
					wait = false; // exit wait loop to fail detection
					LED_RED_On();
					snprintf(imu_buffer, max_buf, "** BNO08X detection failed **");
					imu->init_good = false;
					/*
					 * fake IMU
					 */
#ifdef FAKE_IMU
					imu->update = true;
#endif
				}
				if (imu->update) { // ISR set detection flag
					wait = false;
					snprintf(imu_buffer, max_buf, "BNO08X interrupt detected");

					/*
					 * let's talk to the device
					 */
					bno086_get_header(imu); // first 4 bytes

					snprintf(cmd_buffer, max_buf, "bno086_get_cpacket");
					if (bno086_get_cpacket(SHTP_HEADER_SIZE, imu)) {
						imu->init_good = true;
					} else { // bad or no packet
						imu->init_good = false;
						snprintf(response_buffer, max_buf, "BNO08X bad, no packet");
						LED_RED_On();
						return;
					}

					clearSendBuffer(imu);
					txShtpData[0] = SHTP_REPORT_PRODUCT_ID_REQUEST; //Request the product ID and reset info
					txShtpData[1] = 0; //Reserved
					sendPacket(CHANNEL_CONTROL, 2, imu);

					waitForPacket(CHANNEL_CONTROL, SHTP_REPORT_PRODUCT_ID_RESPONSE, imu);

					if (rxShtpData[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE) {
						majorSoftwareVersion = rxShtpData[2];
						minorSoftwareVersion = rxShtpData[3];
						patchSoftwareVersion = (rxShtpData[13] << 8) | rxShtpData[12];
						partNumber = (rxShtpData[7] << 24) | (rxShtpData[6] << 16) | (rxShtpData[5] << 8) | rxShtpData[4];
						buildNumber = (rxShtpData[11] << 24) | (rxShtpData[10] << 16) | (rxShtpData[9] << 8) | rxShtpData[8];
						snprintf(response_buffer, max_buf, "BNO08X reports version %hhu.%hhu.%hu, build %u, part no. %u\n",
							majorSoftwareVersion, minorSoftwareVersion, patchSoftwareVersion,
							buildNumber, partNumber);
					} else {
						imu->init_good = false;
						LED_RED_On();
						return;
					}
					LED_RED_Off();
					LED_GREEN_On();
				}

			}
		}
	}
}

bool waitForPacket(int channel, uint8_t reportID, void * imup)
{
	imu_cmd_t * imu = imup;

	StartTimer(TMR_BNO_TO, 5);
	while (!TimerDone(TMR_BNO_TO)) {
		if (TimerDone(TMR_BNO_TO)) {
			if (!bno086_receive_packet(imu)) {
				return false;
			}

			if (channel == rxShtpHeader[2] && reportID == rxShtpData[0]) {
				// found correct packet!
				return true;
			} else {
				// other data packet, send to proper channels
				processPacket();
			}
		}
	}

	return false;
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
	IMU_CS_Clear();
	SPI2_WriteRead(imu->tbuf, totalLength, imu->rbuf, totalLength);
	IMU_CS_Set();

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
	size_t currReportOffset = 0;

	currReportOffset += SIZEOF_BASE_TIMESTAMP;

	while (currReportOffset < rxPacketLength) {
		// lots of sensor reports use 3 16-bit numbers stored in bytes 4 through 9
		// we can save some time by parsing those out here.

		uint16_t data1 = (uint16_t) rxShtpData[currReportOffset + 5] << 8 | rxShtpData[currReportOffset + 4];
		uint16_t data2 = (uint16_t) rxShtpData[currReportOffset + 7] << 8 | rxShtpData[currReportOffset + 6];
		uint16_t data3 = (uint16_t) rxShtpData[currReportOffset + 9] << 8 | rxShtpData[currReportOffset + 8];
		uint8_t reportNum = rxShtpData[currReportOffset];

		if (reportNum != SENSOR_REPORTID_TIMESTAMP_REBASE) {
			// set status from byte 2
			reportStatus[reportNum] = (uint8_t) (rxShtpData[currReportOffset + 2] & 0b11);

			// set updated flag
			reportHasBeenUpdated[reportNum] = true;
		}

		switch (rxShtpData[currReportOffset]) {
		case SENSOR_REPORTID_TIMESTAMP_REBASE:
			currReportOffset += SIZEOF_TIMESTAMP_REBASE;
			break;
		case SENSOR_REPORTID_ACCELEROMETER:
			totalAcceleration.v[0] = qToFloat(data1, ACCELEROMETER_Q_POINT);
			totalAcceleration.v[1] = qToFloat(data2, ACCELEROMETER_Q_POINT);
			totalAcceleration.v[2] = qToFloat(data3, ACCELEROMETER_Q_POINT);

			currReportOffset += SIZEOF_ACCELEROMETER;
			break;
		case SENSOR_REPORTID_LINEAR_ACCELERATION:
			linearAcceleration.v[0] = qToFloat(data1, ACCELEROMETER_Q_POINT);
			linearAcceleration.v[1] = qToFloat(data2, ACCELEROMETER_Q_POINT);
			linearAcceleration.v[2] = qToFloat(data3, ACCELEROMETER_Q_POINT);

			currReportOffset += SIZEOF_LINEAR_ACCELERATION;
			break;
		case SENSOR_REPORTID_GRAVITY:
			gravityAcceleration.v[0] = qToFloat(data1, ACCELEROMETER_Q_POINT);
			gravityAcceleration.v[1] = qToFloat(data2, ACCELEROMETER_Q_POINT);
			gravityAcceleration.v[2] = qToFloat(data3, ACCELEROMETER_Q_POINT);

			currReportOffset += SIZEOF_LINEAR_ACCELERATION;
			break;
		case SENSOR_REPORTID_GYROSCOPE_CALIBRATED:
			gyroRotation.v[0] = qToFloat(data1, GYRO_Q_POINT);
			gyroRotation.v[1] = qToFloat(data2, GYRO_Q_POINT);
			gyroRotation.v[2] = qToFloat(data3, GYRO_Q_POINT);

			currReportOffset += SIZEOF_GYROSCOPE_CALIBRATED;
			break;
		case SENSOR_REPORTID_MAGNETIC_FIELD_CALIBRATED:
			magField.v[0] = qToFloat(data1, MAGNETOMETER_Q_POINT);
			magField.v[1] = qToFloat(data2, MAGNETOMETER_Q_POINT);
			magField.v[2] = qToFloat(data3, MAGNETOMETER_Q_POINT);

			currReportOffset += SIZEOF_MAGNETIC_FIELD_CALIBRATED;
			break;
		case SENSOR_REPORTID_MAGNETIC_FIELD_UNCALIBRATED:
		{
			magFieldUncalibrated.v[0] = qToFloat(data1, MAGNETOMETER_Q_POINT);
			magFieldUncalibrated.v[1] = qToFloat(data2, MAGNETOMETER_Q_POINT);
			magFieldUncalibrated.v[2] = qToFloat(data3, MAGNETOMETER_Q_POINT);

			uint16_t ironOffsetXQ = (uint16_t) rxShtpData[currReportOffset + 11] << 8 | rxShtpData[currReportOffset + 10];
			uint16_t ironOffsetYQ = (uint16_t) rxShtpData[currReportOffset + 13] << 8 | rxShtpData[currReportOffset + 12];
			uint16_t ironOffsetZQ = (uint16_t) rxShtpData[currReportOffset + 15] << 8 | rxShtpData[currReportOffset + 14];

			hardIronOffset.v[0] = qToFloat(ironOffsetXQ, MAGNETOMETER_Q_POINT);
			hardIronOffset.v[1] = qToFloat(ironOffsetYQ, MAGNETOMETER_Q_POINT);
			hardIronOffset.v[1] = qToFloat(ironOffsetZQ, MAGNETOMETER_Q_POINT);

			currReportOffset += SIZEOF_MAGNETIC_FIELD_UNCALIBRATED;
		}
			break;
		case SENSOR_REPORTID_ROTATION_VECTOR:
		{
			uint16_t realPartQ = (uint16_t) rxShtpData[currReportOffset + 11] << 8 | rxShtpData[currReportOffset + 10];
			uint16_t accuracyQ = (uint16_t) rxShtpData[currReportOffset + 13] << 8 | rxShtpData[currReportOffset + 12];

			rotationVector.v[0] = qToFloat(data1, ROTATION_Q_POINT);
			rotationVector.v[1] = qToFloat(data2, ROTATION_Q_POINT);
			rotationVector.v[2] = qToFloat(data3, ROTATION_Q_POINT);
			rotationVector.w = qToFloat(realPartQ, ROTATION_Q_POINT);

			rotationAccuracy = qToFloat(accuracyQ, ROTATION_ACCURACY_Q_POINT);

			currReportOffset += SIZEOF_ROTATION_VECTOR;
		}
			break;
		case SENSOR_REPORTID_GAME_ROTATION_VECTOR:
		{
			uint16_t realPartQ = (uint16_t) rxShtpData[currReportOffset + 11] << 8 | rxShtpData[currReportOffset + 10];

			gameRotationVector.v[0] = qToFloat(data1, ROTATION_Q_POINT);
			gameRotationVector.v[1] = qToFloat(data2, ROTATION_Q_POINT);
			gameRotationVector.v[2] = qToFloat(data3, ROTATION_Q_POINT);
			gameRotationVector.w = qToFloat(realPartQ, ROTATION_Q_POINT);

			currReportOffset += SIZEOF_GAME_ROTATION_VECTOR;
		}
			break;
		case SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR:
		{
			uint16_t realPartQ = (uint16_t) rxShtpData[currReportOffset + 11] << 8 | rxShtpData[currReportOffset + 10];
			uint16_t accuracyQ = (uint16_t) rxShtpData[currReportOffset + 13] << 8 | rxShtpData[currReportOffset + 12];

			geomagneticRotationVector.v[0] = qToFloat(data1, ROTATION_Q_POINT);
			geomagneticRotationVector.v[1] = qToFloat(data2, ROTATION_Q_POINT);
			geomagneticRotationVector.v[2] = qToFloat(data3, ROTATION_Q_POINT);
			geomagneticRotationVector.w = qToFloat(realPartQ, ROTATION_Q_POINT);

			geomagneticRotationAccuracy = qToFloat(accuracyQ, ROTATION_ACCURACY_Q_POINT);

			currReportOffset += SIZEOF_GEOMAGNETIC_ROTATION_VECTOR;
		}
			break;
		case SENSOR_REPORTID_TAP_DETECTOR:

			// since we got the report, a tap was detected
			tapDetected = true;

			doubleTap = (rxShtpData[currReportOffset + 4] & (1 << 6)) != 0;

			currReportOffset += SIZEOF_TAP_DETECTOR;
			break;
		case SENSOR_REPORTID_STABILITY_CLASSIFIER:
		{
			uint8_t classificationNumber = rxShtpData[currReportOffset + 4];

			if (classificationNumber > 4) {
				classificationNumber = 0;
			}

			stability = (enum Stability) (classificationNumber);
			currReportOffset += SIZEOF_STABILITY_REPORT;
		}
			break;
		case SENSOR_REPORTID_STEP_DETECTOR:
			// the fact that we got the report means that a step was detected
			stepDetected = true;
			currReportOffset += SIZEOF_STEP_DETECTOR;
			break;
		case SENSOR_REPORTID_STEP_COUNTER:
			stepCount = rxShtpData[currReportOffset + 9] << 8 | rxShtpData[currReportOffset + 8];
			currReportOffset += SIZEOF_STEP_COUNTER;
			break;
		case SENSOR_REPORTID_SIGNIFICANT_MOTION:
			// the fact that we got the report means that significant motion was detected
			significantMotionDetected = true;

			currReportOffset += SIZEOF_SIGNIFICANT_MOTION;
			break;
		case SENSOR_REPORTID_SHAKE_DETECTOR:
			shakeDetected = true;

			xAxisShake = (rxShtpData[currReportOffset + 4] & 1) != 0;
			yAxisShake = (rxShtpData[currReportOffset + 4] & (1 << 1)) != 0;
			zAxisShake = (rxShtpData[currReportOffset + 4] & (1 << 2)) != 0;

			currReportOffset += SIZEOF_SHAKE_DETECTOR;
			break;
		default:
			return;
			break;
		}
	}
}

void bno086_get_header(void * imup)
{
	imu_cmd_t * imu = imup;

	IMU_CS_Clear();
	SPI2_WriteRead(NULL, 0, imu->rbuf, SHTP_HEADER_SIZE);
	IMU_CS_Set();
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

	IMU_CS_Clear();
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
	IMU_CS_Set();

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
	snprintf(imu_buffer, max_buf, "%s Driver %s %s %s ", BNO086_ALIAS, BNO086_DRIVER, build_date, build_time);
}

//Given a register value and a Q point, convert to float
//See https://en.wikipedia.org/wiki/Q_(number_format)

float qToFloat(int16_t fixedPointValue, uint8_t qPoint)
{
	float qFloat = fixedPointValue;
	qFloat *= pow(2.0f, qPoint * -1);
	return(qFloat);
}

float qToFloat_dword(uint32_t fixedPointValue, int16_t qPoint)
{
	float qFloat = fixedPointValue;
	qFloat *= pow(2.0f, qPoint * -1);
	return(qFloat);
}

//Given a floating point value and a Q point, convert to Q
//See https://en.wikipedia.org/wiki/Q_(number_format)

int16_t floatToQ(float qFloat, uint8_t qPoint)
{
	int16_t qVal = (int16_t) (qFloat * pow(2.0f, qPoint));
	return qVal;
}

int32_t floatToQ_dword(float qFloat, uint16_t qPoint)
{
	int32_t qVal = (int32_t) (qFloat * pow(2.0f, qPoint));
	return qVal;
}
