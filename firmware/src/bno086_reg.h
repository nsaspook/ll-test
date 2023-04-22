/* 
 * File:   bno086_reg.h
 * Author: root
 *
 * Created on April 20, 2023, 11:55 AM
 */

#ifndef BNO086_REG_H
#define	BNO086_REG_H

#ifdef	__cplusplus
extern "C" {
#endif

#define BNO08x_SHTP_HDR_SIZE		0x4
#define BNO08x_SHTP_HDR_LEN_LSB		0x0
#define BNO08x_SHTP_HDR_LEN_MSB		0x1
#define BNO08x_SHTP_HDR_CHAN		0x2
#define BNO08x_SHTP_HDR_SEQ		0x3

#define BNO08x_SHTP_MAX_SIZE		128
#define BNO08x_SHTP_ADV_MAX_SIZE	512

#define BNO08x_SHTP_SOFT_RESET		0x01
#define BNO08x_SHTP_PROD_ID		0xf8
#define BNO08x_SHTP_PROD_ID_REQ		0xf9

#define BNO08x_SHTP_GET_FEATURE_RESP	0xfc
#define BNO08x_SHTP_SET_FEATURE_CMD	0xfd
#define BNO08x_SHTP_GET_FEATURE_REQ	0xfe

#define BNO08x_ROT_SCAN_INDEX		0x01
#define BNO08x_REPORTID_ROTATION_VEC	0x05
#define BNO08x_REPORTID_TIMESTAMP_BASE	0xfb

enum bno08x_shtp_channels {
	BNO08x_SHTP_COMMAND_CHAN,
	BNO08x_SHTP_EXECTUABLE_CHAN,
	BNO08x_SHTP_CONTROL_CHAN,
	BNO08x_SHTP_REPORTS_CHAN,
	BNO08x_SHTP_WAKE_REPORTS_CHAN,
	BNO08x_SHTP_GYRO_CHAN,
	BNO08x_NUM_CHANNELS,
};

#define SH2_MAX_SENSOR_EVENT_LEN (60)
typedef struct sh2_SensorEvent {
    uint64_t timestamp_uS;
    int64_t delay_uS;
    uint8_t len;
    uint8_t reportId;
    uint8_t report[SH2_MAX_SENSOR_EVENT_LEN];
} sh2_SensorEvent_t;

typedef void (sh2_SensorCallback_t)(void * cookie, sh2_SensorEvent_t *pEvent);

/**
 * @brief Product Id value
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_ProductId_s {
    uint8_t resetCause;
    uint8_t swVersionMajor;
    uint8_t swVersionMinor;
    uint32_t swPartNumber;
    uint32_t swBuildNumber;
    uint16_t swVersionPatch;
    uint8_t reserved0;
    uint8_t reserved1;
} sh2_ProductId_t;

#define SH2_MAX_PROD_ID_ENTRIES (5)
typedef struct sh2_ProductIds_s {
    sh2_ProductId_t entry[SH2_MAX_PROD_ID_ENTRIES];
    uint8_t numEntries;
} sh2_ProductIds_t;

/**
 * @brief List of sensor types supported by the hub
 *
 * See the SH-2 Reference Manual for more information on each type.
 */
enum sh2_SensorId_e {
    SH2_RAW_ACCELEROMETER = 0x14,
    SH2_ACCELEROMETER = 0x01,
    SH2_LINEAR_ACCELERATION = 0x04,
    SH2_GRAVITY = 0x06,
    SH2_RAW_GYROSCOPE = 0x15,
    SH2_GYROSCOPE_CALIBRATED = 0x02,
    SH2_GYROSCOPE_UNCALIBRATED = 0x07,
    SH2_RAW_MAGNETOMETER = 0x16,
    SH2_MAGNETIC_FIELD_CALIBRATED = 0x03,
    SH2_MAGNETIC_FIELD_UNCALIBRATED = 0x0f,
    SH2_ROTATION_VECTOR = 0x05,
    SH2_GAME_ROTATION_VECTOR = 0x08,
    SH2_GEOMAGNETIC_ROTATION_VECTOR = 0x09,
    SH2_PRESSURE = 0x0a,
    SH2_AMBIENT_LIGHT = 0x0b,
    SH2_HUMIDITY = 0x0c,
    SH2_PROXIMITY = 0x0d,
    SH2_TEMPERATURE = 0x0e,
    SH2_RESERVED = 0x17,
    SH2_TAP_DETECTOR = 0x10,
    SH2_STEP_DETECTOR = 0x18,
    SH2_STEP_COUNTER = 0x11,
    SH2_SIGNIFICANT_MOTION = 0x12,
    SH2_STABILITY_CLASSIFIER = 0x13,
    SH2_SHAKE_DETECTOR = 0x19,
    SH2_FLIP_DETECTOR = 0x1a,
    SH2_PICKUP_DETECTOR = 0x1b,
    SH2_STABILITY_DETECTOR = 0x1c,
    SH2_PERSONAL_ACTIVITY_CLASSIFIER = 0x1e,
    SH2_SLEEP_DETECTOR = 0x1f,
    SH2_TILT_DETECTOR = 0x20,
    SH2_POCKET_DETECTOR = 0x21,
    SH2_CIRCLE_DETECTOR = 0x22,
    SH2_HEART_RATE_MONITOR = 0x23,
    SH2_ARVR_STABILIZED_RV = 0x28,
    SH2_ARVR_STABILIZED_GRV = 0x29,
    SH2_GYRO_INTEGRATED_RV = 0x2A,
    SH2_IZRO_MOTION_REQUEST = 0x2B,
    SH2_RAW_OPTICAL_FLOW = 0x2C,
    SH2_DEAD_RECKONING_POSE = 0x2D,
    SH2_WHEEL_ENCODER = 0x2E,

    // UPDATE to reflect greatest sensor id
    SH2_MAX_SENSOR_ID = 0x2E,
};
typedef uint8_t sh2_SensorId_t;

/**
 * @brief Sensor Configuration settings
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_SensorConfig {
    /* Change sensitivity enabled */
    bool changeSensitivityEnabled;  /**< @brief Enable reports on change */

    /* Change sensitivity - true if relative; false if absolute */
    bool changeSensitivityRelative;  /**< @brief Change reports relative (vs absolute) */

    /* Wake-up enabled */
    bool wakeupEnabled;  /**< @brief Wake host on event */

    /* Always on enabled */
    bool alwaysOnEnabled;  /**< @brief Sensor remains on in sleep state */

    bool sniffEnabled; /**< @brief Any output from this sensor should
                            be sent to host, but reportInterval_us and
                            sensorSpecific do not influence sensor
                            operation. Not supported by all sensors. */

    /* 16-bit signed fixed point integer representing the value a
     * sensor output must exceed in order to trigger another input
     * report. A setting of 0 causes all reports to be sent.
     */
    uint16_t changeSensitivity;  /**< @brief Report-on-change threshold */

    /* Interval in microseconds between asynchronous input reports. */
    uint32_t reportInterval_us;  /**< @brief [uS] Report interval */

    /* Reserved field, not used. */
    uint32_t batchInterval_us;  /**< @brief [uS] Batch interval */

    /* Meaning is sensor specific */
    uint32_t sensorSpecific;  /**< @brief See SH-2 Reference Manual for details. */
} sh2_SensorConfig_t;

/**
 * @brief Sensor Metadata Record
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_SensorMetadata {
    uint8_t meVersion;   /**< @brief Motion Engine Version */
    uint8_t mhVersion;  /**< @brief Motion Hub Version */
    uint8_t shVersion;  /**< @brief SensorHub Version */
    uint32_t range;  /**< @brief Same units as sensor reports */
    uint32_t resolution;  /**< @brief Same units as sensor reports */
    uint16_t revision;  /**< @brief Metadata record format revision */
    uint16_t power_mA;    /**< @brief [mA] Fixed point 16Q10 format */
    uint32_t minPeriod_uS;  /**< @brief [uS] */
    uint32_t maxPeriod_uS;  /**< @brief [uS] */
    uint32_t fifoReserved;  /**< @brief (Unused) */
    uint32_t fifoMax;  /**< @brief (Unused) */
    uint32_t batchBufferBytes;  /**< @brief (Unused) */
    uint16_t qPoint1;     /**< @brief q point for sensor values */
    uint16_t qPoint2;     /**< @brief q point for accuracy or bias fields */
    uint16_t qPoint3;     /**< @brief q point for sensor data change sensitivity */
    uint32_t vendorIdLen; /**< @brief [bytes] */
    char vendorId[48];  /**< @brief Vendor name and part number */
    uint32_t sensorSpecificLen;  /**< @brief [bytes] */
    uint8_t sensorSpecific[48];  /**< @brief See SH-2 Reference Manual */
} sh2_SensorMetadata_t;

/**
 * @brief SensorHub Error Record
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_ErrorRecord {
    uint8_t severity;   /**< @brief Error severity, 0: most severe. */
    uint8_t sequence;   /**< @brief Sequence number (by severity) */
    uint8_t source;     /**< @brief 1-MotionEngine, 2-MotionHub, 3-SensorHub, 4-Chip  */
    uint8_t error;      /**< @brief See SH-2 Reference Manual */
    uint8_t module;     /**< @brief See SH-2 Reference Manual */
    uint8_t code;       /**< @brief See SH-2 Reference Manual */
} sh2_ErrorRecord_t;

/**
 * @brief SensorHub Counter Record
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_Counts {
    uint32_t offered;   /**< @brief [events] */
    uint32_t accepted;  /**< @brief [events] */
    uint32_t on;        /**< @brief [events] */
    uint32_t attempted; /**< @brief [events] */
} sh2_Counts_t;

/**
 * @brief Values for specifying tare basis
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef enum sh2_TareBasis {
    SH2_TARE_BASIS_ROTATION_VECTOR = 0,             /**< @brief Use Rotation Vector */
    SH2_TARE_BASIS_GAMING_ROTATION_VECTOR = 1,      /**< @brief Use Game Rotation Vector */
    SH2_TARE_BASIS_GEOMAGNETIC_ROTATION_VECTOR = 2, /**< @brief Use Geomagnetic R.V. */
} sh2_TareBasis_t;

/**
 * @brief Bit Fields for specifying tare axes.
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef enum sh2_TareAxis {
    SH2_TARE_X = 1,  /**< @brief sh2_tareNow() axes bit field */
    SH2_TARE_Y = 2,  /**< @brief sh2_tareNow() axes bit field */
    SH2_TARE_Z = 4,  /**< @brief sh2_tareNow() axes bit field */
} sh2_TareAxis_t;

/**
 * @brief Quaternion (double precision floating point representation.)
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_Quaternion {
    double x;
    double y;
    double z;
    double w;
} sh2_Quaternion_t;

/**
 * @brief Oscillator type: Internal or External
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef enum {
    SH2_OSC_INTERNAL    = 0,
    SH2_OSC_EXT_CRYSTAL = 1,
    SH2_OSC_EXT_CLOCK   = 2,
} sh2_OscType_t;

/**
 * @brief Calibration result
 *
 * See the SH-2 Reference Manual, Finish Calibration Response.
 */
typedef enum {
    SH2_CAL_SUCCESS = 0,
    SH2_CAL_NO_ZRO,
    SH2_CAL_NO_STATIONARY_DETECTION,
    SH2_CAL_ROTATION_OUTSIDE_SPEC,
    SH2_CAL_ZRO_OUTSIDE_SPEC,
    SH2_CAL_ZGO_OUTSIDE_SPEC,
    SH2_CAL_GYRO_GAIN_OUTSIDE_SPEC,
    SH2_CAL_GYRO_PERIOD_OUTSIDE_SPEC,
    SH2_CAL_GYRO_DROPS_OUTSIDE_SPEC,
} sh2_CalStatus_t;

// FRS Record Ids
#define STATIC_CALIBRATION_AGM                   (0x7979)
#define NOMINAL_CALIBRATION                      (0x4D4D)
#define STATIC_CALIBRATION_SRA                   (0x8A8A)
#define NOMINAL_CALIBRATION_SRA                  (0x4E4E)
#define DYNAMIC_CALIBRATION                      (0x1F1F)
#define ME_POWER_MGMT                            (0xD3E2)
#define SYSTEM_ORIENTATION                       (0x2D3E)
#define ACCEL_ORIENTATION                        (0x2D41)
#define SCREEN_ACCEL_ORIENTATION                 (0x2D43)
#define GYROSCOPE_ORIENTATION                    (0x2D46)
#define MAGNETOMETER_ORIENTATION                 (0x2D4C)
#define ARVR_STABILIZATION_RV                    (0x3E2D)
#define ARVR_STABILIZATION_GRV                   (0x3E2E)
#define TAP_DETECT_CONFIG                        (0xC269)
#define SIG_MOTION_DETECT_CONFIG                 (0xC274)
#define SHAKE_DETECT_CONFIG                      (0x7D7D)
#define MAX_FUSION_PERIOD                        (0xD7D7)
#define SERIAL_NUMBER                            (0x4B4B)
#define ES_PRESSURE_CAL                          (0x39AF)
#define ES_TEMPERATURE_CAL                       (0x4D20)
#define ES_HUMIDITY_CAL                          (0x1AC9)
#define ES_AMBIENT_LIGHT_CAL                     (0x39B1)
#define ES_PROXIMITY_CAL                         (0x4DA2)
#define ALS_CAL                                  (0xD401)
#define PROXIMITY_SENSOR_CAL                     (0xD402)
#define PICKUP_DETECTOR_CONFIG                   (0x1B2A)
#define FLIP_DETECTOR_CONFIG                     (0xFC94)
#define STABILITY_DETECTOR_CONFIG                (0xED85)
#define ACTIVITY_TRACKER_CONFIG                  (0xED88)
#define SLEEP_DETECTOR_CONFIG                    (0xED87)
#define TILT_DETECTOR_CONFIG                     (0xED89)
#define POCKET_DETECTOR_CONFIG                   (0xEF27)
#define CIRCLE_DETECTOR_CONFIG                   (0xEE51)
#define USER_RECORD                              (0x74B4)
#define ME_TIME_SOURCE_SELECT                    (0xD403)
#define UART_FORMAT                              (0xA1A1)
#define GYRO_INTEGRATED_RV_CONFIG                (0xA1A2)
#define DR_IMU_CONFIG                            (0xDED2)
#define DR_VEL_EST_CONFIG                        (0xDED3)
#define DR_SYNC_CONFIG                           (0xDED4)
#define DR_QUAL_CONFIG                           (0xDED5)
#define DR_CAL_CONFIG                            (0xDED6)
#define DR_LIGHT_REC_CONFIG                      (0xDED8)
#define DR_FUSION_CONFIG                         (0xDED9)
#define DR_OF_CONFIG                             (0xDEDA)
#define DR_WHEEL_CONFIG                          (0xDEDB)
#define DR_CAL                                   (0xDEDC)
#define DR_WHEEL_SELECT                          (0xDEDF)
#define FRS_ID_META_RAW_ACCELEROMETER            (0xE301)
#define FRS_ID_META_ACCELEROMETER                (0xE302)
#define FRS_ID_META_LINEAR_ACCELERATION          (0xE303)
#define FRS_ID_META_GRAVITY                      (0xE304)
#define FRS_ID_META_RAW_GYROSCOPE                (0xE305)
#define FRS_ID_META_GYROSCOPE_CALIBRATED         (0xE306)
#define FRS_ID_META_GYROSCOPE_UNCALIBRATED       (0xE307)
#define FRS_ID_META_RAW_MAGNETOMETER             (0xE308)
#define FRS_ID_META_MAGNETIC_FIELD_CALIBRATED    (0xE309)
#define FRS_ID_META_MAGNETIC_FIELD_UNCALIBRATED  (0xE30A)
#define FRS_ID_META_ROTATION_VECTOR              (0xE30B)
#define FRS_ID_META_GAME_ROTATION_VECTOR         (0xE30C)
#define FRS_ID_META_GEOMAGNETIC_ROTATION_VECTOR  (0xE30D)
#define FRS_ID_META_PRESSURE                     (0xE30E)
#define FRS_ID_META_AMBIENT_LIGHT                (0xE30F)
#define FRS_ID_META_HUMIDITY                     (0xE310)
#define FRS_ID_META_PROXIMITY                    (0xE311)
#define FRS_ID_META_TEMPERATURE                  (0xE312)
#define FRS_ID_META_TAP_DETECTOR                 (0xE313)
#define FRS_ID_META_STEP_DETECTOR                (0xE314)
#define FRS_ID_META_STEP_COUNTER                 (0xE315)
#define FRS_ID_META_SIGNIFICANT_MOTION           (0xE316)
#define FRS_ID_META_STABILITY_CLASSIFIER         (0xE317)
#define FRS_ID_META_SHAKE_DETECTOR               (0xE318)
#define FRS_ID_META_FLIP_DETECTOR                (0xE319)
#define FRS_ID_META_PICKUP_DETECTOR              (0xE31A)
#define FRS_ID_META_STABILITY_DETECTOR           (0xE31B)
#define FRS_ID_META_PERSONAL_ACTIVITY_CLASSIFIER (0xE31C)
#define FRS_ID_META_SLEEP_DETECTOR               (0xE31D)
#define FRS_ID_META_TILT_DETECTOR                (0xE31E)
#define FRS_ID_META_POCKET_DETECTOR              (0xE31F)
#define FRS_ID_META_CIRCLE_DETECTOR              (0xE320)
#define FRS_ID_META_HEART_RATE_MONITOR           (0xE321)
#define FRS_ID_META_ARVR_STABILIZED_RV           (0xE322)
#define FRS_ID_META_ARVR_STABILIZED_GRV          (0xE323)
#define FRS_ID_META_GYRO_INTEGRATED_RV           (0xE324)
#define FRS_ID_META_RAW_OPTICAL_FLOW             (0xE326)

/**
 * @brief Interactive ZRO Motion Intent
 *
 * See the SH-2 Reference Manual, 6.4.13
 */
typedef enum {
    SH2_IZRO_MI_UNKNOWN = 0,
    SH2_IZRO_MI_STATIONARY_NO_VIBRATION,
    SH2_IZRO_MI_STATIONARY_WITH_VIBRATION,
    SH2_IZRO_MI_IN_MOTION,
    SH2_IZRO_MI_ACCELERATING,
} sh2_IZroMotionIntent_t;

/**
 * @brief Interactive ZRO Motion Intent
 *
 * See the SH-2 Reference Manual, 6.4.13
 */
typedef enum {
    SH2_IZRO_MR_NO_REQUEST = 0,
    SH2_IZRO_MR_STAY_STATIONARY,
    SH2_IZRO_MR_STATIONARY_NON_URGENT,
    SH2_IZRO_MR_STATIONARY_URGENT,
} sh2_IZroMotionRequest_t;


/**
* @brief Asynchronous Event
*
* Represents reset events and other non-sensor events received from SH-2 sensor hub.
*/

enum sh2_AsyncEventId_e {
    SH2_RESET,
    SH2_SHTP_EVENT,
    SH2_GET_FEATURE_RESP,
};
typedef enum sh2_AsyncEventId_e sh2_AsyncEventId_t;

enum sh2_ShtpEvent_e {
    SH2_SHTP_TX_DISCARD = 0,
    SH2_SHTP_SHORT_FRAGMENT = 1,
    SH2_SHTP_TOO_LARGE_PAYLOADS = 2,
    SH2_SHTP_BAD_RX_CHAN = 3,
    SH2_SHTP_BAD_TX_CHAN = 4,
    SH2_SHTP_BAD_FRAGMENT = 5,
    SH2_SHTP_BAD_SN = 6,
    SH2_SHTP_INTERRUPTED_PAYLOAD = 7,
};
typedef uint8_t sh2_ShtpEvent_t;

typedef struct sh2_SensorConfigResp_e {
    sh2_SensorId_t sensorId;
    sh2_SensorConfig_t sensorConfig;
} sh2_SensorConfigResp_t;

typedef struct sh2_AsyncEvent {
    uint32_t eventId;
    union {
        sh2_ShtpEvent_t shtpEvent;
        sh2_SensorConfigResp_t sh2SensorConfigResp;
    };
} sh2_AsyncEvent_t;

typedef enum shtp_Event_e {
    SHTP_SHORT_FRAGMENT = 1,
    SHTP_TOO_LARGE_PAYLOADS = 2,
    SHTP_BAD_RX_CHAN = 3,
    SHTP_BAD_TX_CHAN = 4,
    SHTP_BAD_FRAGMENT = 5,
    SHTP_BAD_SN = 6,
    SHTP_INTERRUPTED_PAYLOAD = 7,
} shtp_Event_t;

/* Note on quaternion naming conventions:
 * Quaternions are values with four real components that are usually
 * interpreted as coefficients in the complex quantity, Q.
 *
 * As in, Q = W + Xi + Yj + Zk
 *
 * Where i, j and k represent the three imaginary dimensions.
 *
 * So W represents the Real components and X, Y and Z the Imaginary ones.
 *
 * In the Hillcrest datasheets and in this code, however, the four components
 * are named real, i, j and k, to make it explicit which is which.  If you 
 * need to translate these names into the "wxyz" or "xyzw" convention, then, the
 * appropriate mapping is this:
 *     w = real
 *     x = i
 *     y = j
 *     z = k
 */
	
/**
 * @brief Raw Accelerometer
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_RawAccelerometer {
    /* Units are ADC counts */
    int16_t x;  /**< @brief [ADC counts] */
    int16_t y;  /**< @brief [ADC counts] */
    int16_t z;  /**< @brief [ADC counts] */

    /* Microseconds */
    uint32_t timestamp;  /**< @brief [uS] */
} sh2_RawAccelerometer_t;

/**
 * @brief Accelerometer
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_Accelerometer {
    float x;
    float y;
    float z;
} sh2_Accelerometer_t;

/**
 * @brief Raw gyroscope
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_RawGyroscope {
    /* Units are ADC counts */
    int16_t x;  /**< @brief [ADC Counts] */
    int16_t y;  /**< @brief [ADC Counts] */
    int16_t z;  /**< @brief [ADC Counts] */
    int16_t temperature;  /**< @brief [ADC Counts] */

    /* Microseconds */
    uint32_t timestamp;  /**< @brief [uS] */
} sh2_RawGyroscope_t;

/**
 * @brief Gyroscope
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_Gyroscope {
    /* Units are rad/s */
    float x;
    float y;
    float z;
} sh2_Gyroscope_t;

/**
 * @brief Uncalibrated gyroscope
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_GyroscopeUncalibrated {
    /* Units are rad/s */
    float x;  /**< @brief [rad/s] */
    float y;  /**< @brief [rad/s] */
    float z;  /**< @brief [rad/s] */
    float biasX;  /**< @brief [rad/s] */
    float biasY;  /**< @brief [rad/s] */
    float biasZ;  /**< @brief [rad/s] */
} sh2_GyroscopeUncalibrated_t;

/**
 * @brief Raw Magnetometer
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_RawMagnetometer {
    /* Units are ADC counts */
    int16_t x;  /**< @brief [ADC Counts] */
    int16_t y;  /**< @brief [ADC Counts] */
    int16_t z;  /**< @brief [ADC Counts] */

    /* Microseconds */
    uint32_t timestamp;  /**< @brief [uS] */
} sh2_RawMagnetometer_t;

/**
 * @brief Magnetic field
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_MagneticField {
    /* Units are uTesla */
    float x;  /**< @brief [uTesla] */
    float y;  /**< @brief [uTesla] */
    float z;  /**< @brief [uTesla] */
} sh2_MagneticField_t;

/**
 * @brief Uncalibrated magnetic field
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_MagneticFieldUncalibrated {
    /* Units are uTesla */
    float x;  /**< @brief [uTesla] */
    float y;  /**< @brief [uTesla] */
    float z;  /**< @brief [uTesla] */
    float biasX;  /**< @brief [uTesla] */
    float biasY;  /**< @brief [uTesla] */
    float biasZ;  /**< @brief [uTesla] */
} sh2_MagneticFieldUncalibrated_t;

/**
 * @brief Rotation Vector with Accuracy
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_RotationVectorWAcc {
    float i;  /**< @brief Quaternion component i */
    float j;  /**< @brief Quaternion component j */
    float k;  /**< @brief Quaternion component k */
    float real;  /**< @brief Quaternion component, real */
    float accuracy;  /**< @brief Accuracy estimate [radians] */
} sh2_RotationVectorWAcc_t;

/**
 * @brief Rotation Vector
 *
 * See the SH-2 Reference Manual for more detail.
 */
typedef struct sh2_RotationVector {
    float i;  /**< @brief Quaternion component i */
    float j;  /**< @brief Quaternion component j */
    float k;  /**< @brief Quaternion component k */
    float real;  /**< @brief Quaternion component real */
} sh2_RotationVector_t;


#ifdef	__cplusplus
}
#endif

#endif	/* BNO086_REG_H */

