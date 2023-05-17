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
uint8_t dummy_header[512];

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

static bool first = true;

/**
 * Auxiliary accuracy readout from the Rotation Vector report.
 * Represents the estimated accuracy of the rotation vector in radians.
 */
float rotationAccuracy, geomagneticRotationAccuracy;

bool bno086_updateData(void)
{

	if (!bno086_receive_packet(&imu0)) {
		// comms error
		return false;
	}

	processPacket();

	// packets were received, so data may have changed
	return true;
}

/*
 * 
 */
void softreset(void)
{
	txShtpData[0] = 1;

	sendPacket(CHANNEL_EXECUTABLE, 1, &imu0);
}


//Sends the packet to enable reports

void enableReport(enum Report report, uint16_t timeBetweenReports)
{

	// check time is valid
	float periodSeconds = (float) (timeBetweenReports / 1000.0);

	snprintf(imu_buffer, max_buf, "BNO08X time %.06f s", periodSeconds);
	dprintf("%s\r\n", imu_buffer);

	setFeatureCommand((uint8_t) report, timeBetweenReports, 0);

	// note: we don't wait for ACKs on these packets because they can take quite a while, like half a second, to come in
}

void setFeatureCommand(uint8_t reportID, uint16_t timeBetweenReports, uint32_t specificConfig)
{
	uint32_t microsBetweenReports = (uint32_t) (timeBetweenReports * 1000);

	const uint32_t batchMicros = 0;

	txShtpData[0] = SHTP_REPORT_SET_FEATURE_COMMAND; //Set feature command. Reference page 55
	txShtpData[1] = reportID; //Feature Report ID. 0x01 = Accelerometer, 0x05 = Rotation vector
	txShtpData[2] = 0; //Feature flags
	txShtpData[3] = 0; //Change sensitivity (LSB)
	txShtpData[4] = 0; //Change sensitivity (MSB)
	txShtpData[5] = (microsBetweenReports >> 0) & 0xFF; //Report interval (LSB) in microseconds. 0x7A120 = 500ms
	txShtpData[6] = (microsBetweenReports >> 8) & 0xFF; //Report interval
	txShtpData[7] = (microsBetweenReports >> 16) & 0xFF; //Report interval
	txShtpData[8] = (microsBetweenReports >> 24) & 0xFF; //Report interval (MSB)
	txShtpData[9] = (batchMicros >> 0) & 0xFF; //Batch Interval (LSB)
	txShtpData[10] = (batchMicros >> 8) & 0xFF; //Batch Interval
	txShtpData[11] = (batchMicros >> 16) & 0xFF; //Batch Interval
	txShtpData[12] = (batchMicros >> 24) & 0xFF; //Batch Interval (MSB)
	txShtpData[13] = (specificConfig >> 0) & 0xFF; //Sensor-specific config (LSB)
	txShtpData[14] = (specificConfig >> 8) & 0xFF; //Sensor-specific config
	txShtpData[15] = (specificConfig >> 16) & 0xFF; //Sensor-specific config
	txShtpData[16] = (specificConfig >> 24) & 0xFF; //Sensor-specific config (MSB)

	//Transmit packet on channel 2, 17 bytes
	sendPacket(CHANNEL_CONTROL, 17, &imu0);
}

void productIdReg(void)
{

	txShtpData[0] = SHTP_REPORT_PRODUCT_ID_REQUEST; //Set feature command. Reference page 55
	txShtpData[1] = 0;

	//Transmit packet on channel 2, 2 bytes
	sendPacket(CHANNEL_CONTROL, 2, &imu0);
}

void clearSendBuffer(void * imup)
{
	imu_cmd_t * imu = imup;

	memset(imu->tbuf, 0, 256);
}

void bno086_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	bool wait = true;

	/*
	 * power boot/reset
	 */
	IMU_CS_Clear(); // enable IMU
	WaitMs(800);
	//	DIS_RESET_Clear();
	WaitMs(30);
	//	DIS_RESET_Set();
	WaitMs(170);


	dprintf("\r\nbno086 setup start\r\n");
	// set SPI MODE
	set_imu_bits(); // set 8 or 32-bit SPI transfers
	LED_GREEN_Off();
	LED_RED_Off();
	softreset();
	if (imu) {
		if (first) { // power up restart device detection mode
			imu->update = false;
		}
		/*
		 * trigger ISR on IMU data update interrupts
		 */
		init_imu_int_bno(imu);
		snprintf(cmd_buffer, max_buf, "init_imu_int_bno");
		dprintf("%s\r\n", cmd_buffer);

		if (first) { // lets see if the device is alive and talking
			StartTimer(TMR_BNO_INT, BNO_INIT_DELAY_INT);
			while (wait) {
				if (TimerDone(TMR_BNO_INT)) { // timeout waiting for device
					wait = false; // exit wait loop to fail detection
					LED_RED_On();
					snprintf(imu_buffer, max_buf, "** BNO08X detection failed **");
					dprintf("%s\r\n", imu_buffer);
					imu->init_good = false;
					/*
					 * fake IMU interrupt for testing
					 */
#ifdef FAKE_IMU
					imu->update = true;
#endif
				}
				if (imu->update) { // ISR set detection flag
					wait = false;
					snprintf(imu_buffer, max_buf, "BNO08X interrupt detected");
					dprintf("%s\r\n", imu_buffer);
				}

				LED_RED_Off();
				LED_GREEN_On();
			}

		}
		dprintf("\r\nSoft Reset\r\n");
		softreset();
		productIdReg();
		/*
		 * eat any IMU boot messages
		 */
		dprintf("\r\nEmpty IMU message buffer\r\n");
		wait = false;
		StartTimer(TMR_BNO_INT, BNO_INIT_DELAY_INT);
		while (wait && (!TimerDone(TMR_BNO_INT) || imu->update)) {
			if (imu->update) {
				StartTimer(TMR_BNO_INT, BNO_INIT_DELAY_INT);
				imu->update = false;
				clearSendBuffer(imu);
				IMU_CS_Clear();
				SPI2_WriteRead(imu->tbuf, 5, imu->rbuf, 5);
				IMU_CS_Set();
				dprintf("%x %x %x %x %x %c\r\n", imu0.rbuf[0], imu0.rbuf[1], imu0.rbuf[2], imu0.rbuf[3], imu0.rbuf[4], isprint(imu0.rbuf[4]) ? imu0.rbuf[4] : ' ');
			}
		}
		dprintf("\r\nbno086 setup done\r\n");
		first = false;
	}
}

void get_id_dummy(void)
{
	txShtpData[0] = SHTP_REPORT_PRODUCT_ID_REQUEST; //Request the product ID and reset info
	txShtpData[1] = 0; //Reserved
	sendPacket(CHANNEL_CONTROL, 2, &imu0);

	waitForPacket(CHANNEL_CONTROL, SHTP_REPORT_PRODUCT_ID_RESPONSE, &imu0);
	snprintf(cmd_buffer, max_buf, "BNO08X waitForPacket");
	dprintf("%s\r\n", cmd_buffer);

	if (rxShtpData[0] == SHTP_REPORT_PRODUCT_ID_RESPONSE) {
		majorSoftwareVersion = rxShtpData[2];
		minorSoftwareVersion = rxShtpData[3];
		patchSoftwareVersion = (rxShtpData[13] << 8) | rxShtpData[12];
		partNumber = (rxShtpData[7] << 24) | (rxShtpData[6] << 16) | (rxShtpData[5] << 8) | rxShtpData[4];
		buildNumber = (rxShtpData[11] << 24) | (rxShtpData[10] << 16) | (rxShtpData[9] << 8) | rxShtpData[8];
		snprintf(response_buffer, max_buf, "\r\nBNO08X reports version %hhu.%hhu.%hu, build %u, part no. %u\r\n",
			majorSoftwareVersion, minorSoftwareVersion, patchSoftwareVersion,
			buildNumber, partNumber);
		dprintf("%s\r\n", response_buffer);
	} else {
		snprintf(imu_buffer, max_buf, "BNO08X bad ID 0x%X", rxShtpData[0]);
		dprintf("%s\r\n", response_buffer);
		LED_RED_On();
		return;
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
	imu->tbuf[3] = ++sequenceNumber[channelNumber];

	// wipe out any existing RX packet header so we know if we received a packet
	memset(imu->rbuf, 0, SHTP_HEADER_SIZE);

	// send packet to IMU.
	// This also might receive the first part of another packet, which there is no way to avoid.
	TP3_Clear();
	WaitMs(1);
	TP3_Set();
	IMU_CS_Clear();
	SPI2_WriteRead(imu->tbuf, totalLength, imu->rbuf, totalLength);
	IMU_CS_Set();
	delay_us(100);

	if (imu->rbuf[0] == 0 && imu->rbuf[0] == 0) {
		// no header data so no packet received
		return true;
	} else {
		// received first part of data packet while writing
		if (bno086_get_cpacket(totalLength, imu)) {
			// received data packet, send to proper channels
			if (!first) {
				processPacket();
			}
			return true;
		}

		// receive failed
		return false;
	}
}

void processPacket(void)
{
	dprintf("processPacket\r\n");
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
	dprintf("parseSensorDataPacket\r\n");
	size_t currReportOffset = 0;

	currReportOffset += SIZEOF_BASE_TIMESTAMP;

	while (currReportOffset < rxPacketLength) {
		TP3_Toggle(); // debug signal
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
			dprintf("SENSOR_REPORTID_TIMESTAMP_REBASE\r\n");
			currReportOffset += SIZEOF_TIMESTAMP_REBASE;
			break;
		case SENSOR_REPORTID_ACCELEROMETER:
			dprintf("SENSOR_REPORTID_ACCELEROMETER\r\n");
			totalAcceleration.v[0] = qToFloat(data1, ACCELEROMETER_Q_POINT);
			totalAcceleration.v[1] = qToFloat(data2, ACCELEROMETER_Q_POINT);
			totalAcceleration.v[2] = qToFloat(data3, ACCELEROMETER_Q_POINT);
			imu0.accel_report = true; // send CAN data after this is updated

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

			dprintf("SENSOR_REPORTID_ROTATION_VECTOR\r\n");
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
	SPI2_WriteRead(dummy_header, SHTP_HEADER_SIZE, imu->rbuf, SHTP_HEADER_SIZE);
	IMU_CS_Set();
}

// check for new data and read contents into imu.rbuf

bool bno086_receive_packet(void * imup)
{
	imu_cmd_t * imu = imup;

	bno086_get_header(imu); // first 4 bytes
	delay_us(100);
	if (bno086_get_cpacket(SHTP_HEADER_SIZE, imu)) {
		dprintf("bno086_receive_packet\r\n");
		delay_us(100);
		processPacket();
		return true;
	}
	return false;
}

bool bno086_get_cpacket(size_t read_b, void * imup)
{
	imu_cmd_t * imu = imup;
	uint16_t totalLength;

	dprintf("bno086_get_packet\r\n");
	if (imu->rbuf[0] == 0xff && imu->rbuf[1] == 0xff) { // check for invalid device data
		LED_RED_On(); // 
		snprintf(response_buffer, max_buf, "BNO08X bad, invalid data");
		return false;
	}

	//Calculate the number of data bytes in this packet
	totalLength = (uint16_t) imu->rbuf[0] + (((uint16_t) imu->rbuf[1]) << 8);

	// Clear the MSbit.
	totalLength &= ~(1 << 15);

	if (totalLength == 0) {
		// Packet is empty
		snprintf(response_buffer, max_buf, "BNO08X empty packet");
		dprintf("%s\r\n", response_buffer);
		return(false); //All done
	}

	rxPacketLength = totalLength - SHTP_HEADER_SIZE;

	if (totalLength <= read_b) {
		// the original transaction already read the completed packet!  We're done.
		return true;
	}

	if (rxPacketLength > SHTP_RX_PACKET_SIZE) {
		snprintf(response_buffer, max_buf, "BNO08X long packet 0x%x > 0x%x", rxPacketLength, SHTP_RX_PACKET_SIZE);
		dprintf("%s\r\n", response_buffer);
		return false;
	}

	snprintf(response_buffer, max_buf, "BNO08X packet 0x%x 0x%x", rxPacketLength, SHTP_RX_PACKET_SIZE);
	dprintf("%s\r\n", response_buffer);

	IMU_CS_Clear();
	if (read_b == SHTP_HEADER_SIZE) {
		// just read the entire packet into the buffer
		SPI2_WriteRead(dummy_header, totalLength, imu->rbuf, totalLength);
		dprintf("Read entire packet into rbuf\r\n");
	} else {
		// we want to receive a new header, plus the remaining data bytes that haven't been read.
		size_t receiveLength = SHTP_HEADER_SIZE + (totalLength - read_b);

		// read remaining bytes into the data buffer starting at the next byte
		SPI2_WriteRead(dummy_header, receiveLength, imu->rbuf + read_b, receiveLength);

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
		imu->device = IMU_BNO086;
		imu->angles = false;
		imu->online = true;
		return true;
	}
	return false;
}

bool bno086_getdata(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {

		bno086_receive_packet(imu);

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

double qToFloat(int16_t fixedPointValue, uint8_t qPoint)
{
	double qFloat = fixedPointValue;
	qFloat *= pow(2.0f, -qPoint);
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

/*
 * dummy printf replacement for debug lines
 */
int printf_stub(const char* s, ...)
{
	return 1; // dummy printed value
}