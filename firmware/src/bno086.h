/* 
 * File:   bno086.h
 * Author: root
 *
 * Created on April 20, 2023, 11:51 AM
 * code model used and copied https://os.mbed.com/users/MultipleMonomials/code/BNO080/
 * https://github.com/ceva-dsp/sh2
 * 
 */

#ifndef BNO086_H
#define	BNO086_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <math.h>
#include "definitions.h"                // SYS function prototypes
#include "imupic32mcj.h"
#include "bno086_reg.h"
#include "imu.h"

#define BNO086_DRIVER	"V1.000" 
#define BNO086_ALIAS	"BNO086"

#define SQRT_2 1.414213562f

#define BNO_INIT_DELAY_INT	200

#define SHTP_HEADER_SIZE 4

	// Size of the largest individual packet we can receive.
	// Min value is set by the advertisement packet (272 bytes)
	// If you enable lots of sensor reports and get an error, you might need to increase this.
#define SHTP_RX_PACKET_SIZE 272

	// Size of largest packet that we need to transmit (not including header)
#define SHTP_MAX_TX_PACKET_SIZE 17


	// Channels
#define CHANNEL_COMMAND 0
#define CHANNEL_EXECUTABLE 1
#define CHANNEL_CONTROL 2
#define CHANNEL_REPORTS 3
#define CHANNEL_WAKE_REPORTS 4
#define CHANNEL_GYRO 5

	// Report IDs on the command channel.
	// Unlike the other constants, these come from the Sensor Hub Transport Protocol datasheet, section 5.1
#define COMMAND_REPORTID_ADVERTISEMENT 0x0
#define COMMAND_REPORTID_ERRORLIST 0x1

	//All the ways we can configure or talk to the BNO080, figure 34, page 36 reference manual
	//These are used for low level communication with the sensor, on channel 2
#define SHTP_REPORT_COMMAND_RESPONSE 0xF1
#define SHTP_REPORT_COMMAND_REQUEST 0xF2
#define SHTP_REPORT_FRS_READ_RESPONSE 0xF3
#define SHTP_REPORT_FRS_READ_REQUEST 0xF4
#define SHTP_REPORT_FRS_WRITE_RESPONSE 0xF5
#define SHTP_REPORT_FRS_WRITE_DATA 0xF6
#define SHTP_REPORT_FRS_WRITE_REQUEST 0xF7
#define SHTP_REPORT_PRODUCT_ID_RESPONSE 0xF8
#define SHTP_REPORT_PRODUCT_ID_REQUEST 0xF9
#define SHTP_REPORT_BASE_TIMESTAMP 0xFB
#define SHTP_REPORT_SET_FEATURE_COMMAND 0xFD
#define SHTP_REPORT_GET_FEATURE_RESPONSE 0xFC

	//All the different sensors and features we can get reports from
	//These are used when enabling a given sensor
#define SENSOR_REPORTID_TIMESTAMP_REBASE 0xFA
#define SENSOR_REPORTID_ACCELEROMETER 0x01
#define SENSOR_REPORTID_GYROSCOPE_CALIBRATED 0x02
#define SENSOR_REPORTID_MAGNETIC_FIELD_CALIBRATED 0x03
#define SENSOR_REPORTID_LINEAR_ACCELERATION 0x04
#define SENSOR_REPORTID_ROTATION_VECTOR 0x05
#define SENSOR_REPORTID_GRAVITY 0x06
#define SENSOR_REPORTID_GAME_ROTATION_VECTOR 0x08
#define SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR 0x09
#define SENSOR_REPORTID_MAGNETIC_FIELD_UNCALIBRATED 0x0F
#define SENSOR_REPORTID_TAP_DETECTOR 0x10
#define SENSOR_REPORTID_STEP_COUNTER 0x11
#define SENSOR_REPORTID_SIGNIFICANT_MOTION 0x12
#define SENSOR_REPORTID_STABILITY_CLASSIFIER 0x13
#define SENSOR_REPORTID_STEP_DETECTOR 0x18
#define SENSOR_REPORTID_SHAKE_DETECTOR 0x19

	// sensor report ID with the largest numeric value
#define MAX_SENSOR_REPORTID SENSOR_REPORTID_SHAKE_DETECTOR

	// Q points for various sensor data elements
#define ACCELEROMETER_Q_POINT 8 // for accelerometer based data
#define GYRO_Q_POINT 9 // for gyroscope data
#define MAGNETOMETER_Q_POINT 4 // for magnetometer data
#define ROTATION_Q_POINT 14 // for rotation data
#define ROTATION_ACCURACY_Q_POINT 12 // for rotation accuracy data
#define POWER_Q_POINT 10 // for power information in the metadata
#define ORIENTATION_QUAT_Q_POINT 14 // for the set orientation command
#define FRS_ORIENTATION_Q_POINT 30 // for the sensor orientation FRS record

	// Report IDs on the Executable channel
	// See Figure 1-27 in the BNO080 datasheet
#define EXECUTABLE_REPORTID_RESET 0x1

	//Record IDs from SH-2 figure 28
	//These are used to read and set various configuration options
#define FRS_RECORDID_SERIAL_NUMBER 0x4B4B
#define FRS_RECORDID_SYSTEM_ORIENTATION 0x2D3E

	//Command IDs from section 6.4, page 42
	//These are used to calibrate, initialize, set orientation, tare etc the sensor
#define COMMAND_ERRORS 1
#define COMMAND_COUNTER 2
#define COMMAND_TARE 3
#define COMMAND_INITIALIZE 4
#define COMMAND_SAVE_DCD 6
#define COMMAND_ME_CALIBRATE 7
#define COMMAND_DCD_PERIOD_SAVE 9
#define COMMAND_OSCILLATOR 10
#define COMMAND_CLEAR_DCD 11
#define COMMAND_UNSOLICITED_INITIALIZE 0x84

#define CALIBRATE_ACCEL 0
#define CALIBRATE_GYRO 1
#define CALIBRATE_MAG 2
#define CALIBRATE_PLANAR_ACCEL 3
#define CALIBRATE_ACCEL_GYRO_MAG 4
#define CALIBRATE_STOP 5

#define BNO080_RESET_TIMEOUT 180ms

	/// List of all sensor reports that the IMU supports.

	enum Report {
		TOTAL_ACCELERATION = SENSOR_REPORTID_ACCELEROMETER,
		LINEAR_ACCELERATION = SENSOR_REPORTID_LINEAR_ACCELERATION,
		GRAVITY_ACCELERATION = SENSOR_REPORTID_GRAVITY,
		GYROSCOPE = SENSOR_REPORTID_GYROSCOPE_CALIBRATED,
		MAG_FIELD = SENSOR_REPORTID_MAGNETIC_FIELD_CALIBRATED,
		MAG_FIELD_UNCALIBRATED = SENSOR_REPORTID_MAGNETIC_FIELD_UNCALIBRATED,
		ROTATION = SENSOR_REPORTID_ROTATION_VECTOR,
		GEOMAGNETIC_ROTATION = SENSOR_REPORTID_GEOMAGNETIC_ROTATION_VECTOR,
		GAME_ROTATION = SENSOR_REPORTID_GAME_ROTATION_VECTOR,
		TAP_DETECTOR = SENSOR_REPORTID_TAP_DETECTOR,
		STABILITY_CLASSIFIER = SENSOR_REPORTID_STABILITY_CLASSIFIER,
		STEP_DETECTOR = SENSOR_REPORTID_STEP_DETECTOR,
		STEP_COUNTER = SENSOR_REPORTID_STEP_COUNTER,
		SIGNIFICANT_MOTION = SENSOR_REPORTID_SIGNIFICANT_MOTION,
		SHAKE_DETECTOR = SENSOR_REPORTID_SHAKE_DETECTOR
	};

	enum Stability {
		/// Unknown stability type.
		UNKNOWN = 0,

		/// At rest on a stable surface with very little motion
		ON_TABLE = 1,

		/// Motion is stable, but the duration requirement for stability has not been met.
		/// Can only occur during gyroscope calibration (why? beats me!)
		STATIONARY = 2,

		/// Stable (has been below the acceleration threshold for the required duration)
		STABLE = 3,

		/// IMU is moving.
		MOTION = 4
	};

	/*
	 * function pointer templates
	 */
	void bno086_set_spimode(void *);
	bool bno086_getid(void *);
	bool bno086_getdata(void *);
	void bno086_version(void);
	bool bno086_get_cpacket(size_t, void *);
	void bno086_get_header(void *);
	bool bno086_receive_packet(void *);
	void processPacket(void);
	void parseSensorDataPacket(void);
	bool sendPacket(uint8_t, uint8_t, void *);
	bool waitForPacket(int, uint8_t, void *);
	void clearSendBuffer(void *);

	float qToFloat(int16_t, uint8_t);
	float qToFloat_dword(uint32_t, int16_t);
	int16_t floatToQ(float, uint8_t);
	int32_t floatToQ_dword(float t, uint16_t);

	/*
	 * BNO086 chip instance
	 */
	extern imu_cmd_t imu0;
	extern sh2_Quaternion_t fusion_q;



#ifdef	__cplusplus
}
#endif

#endif	/* BNO086_H */

