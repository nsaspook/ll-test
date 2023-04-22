#include "bma490l.h"

static uint8_t R_ID_CMD[BMA490_ID_LEN + 1] = {CHIP_ID | RBIT};
static uint8_t R_IS_CMD[BMA490_ID_LEN + 1] = {CHIP_IS | RBIT};
uint8_t R_ST_CMD[BMA490_ID_LEN + 1] = {CHIP_STATUS | RBIT};
#ifdef BMA400
static uint8_t R_DATA_CMD[BMA400_DATA_BUFFER_LEN] = {BMA400_DATA_INDEX | RBIT, BMA400_DATA_LEN};
#else
static uint8_t R_DATA_CMD[BMA490_DATA_BUFFER_LEN] = {BMA490_DATA_INDEX | RBIT, BMA490_DATA_LEN};
#endif

static bool imu_cs(imu_cmd_t *);
static void imu_cs_cb(uintptr_t);
static void imu_cs_disable(imu_cmd_t *);

void imu_set_reg(imu_cmd_t *, const uint8_t, const uint8_t, const bool);
void imu_get_reg(imu_cmd_t *, const uint8_t, const bool);

static const char *build_date = __DATE__, *build_time = __TIME__;

#ifndef BMA400
/*
 * for any/no motion interrupt IMU features
 */

static const uint8_t bma490l_config_file[] = {
	BMA490L_FEATURE_CONFIG_ADDR,
	0xc8, 0x2e, 0x00, 0x2e, 0xc8, 0x2e, 0x00, 0x2e, 0xc8, 0x2e, 0x00, 0x2e, 0xc8, 0x2e, 0x00, 0x2e, 0xc8, 0x2e, 0x00,
	0x2e, 0xc8, 0x2e, 0x00, 0x2e, 0xc8, 0x2e, 0x00, 0x2e, 0x80, 0x2e, 0x58, 0x01, 0x80, 0x2e, 0x74, 0x02, 0xb0, 0xf0,
	0x10, 0x30, 0x21, 0x2e, 0x16, 0xf0, 0x80, 0x2e, 0xeb, 0x00, 0x19, 0x50, 0x17, 0x52, 0x01, 0x42, 0x3b, 0x80, 0x41,
	0x30, 0x01, 0x42, 0x3c, 0x80, 0x00, 0x2e, 0x01, 0x40, 0x01, 0x42, 0x21, 0x2e, 0xff, 0xaf, 0xb8, 0x2e, 0x9b, 0x95,
	0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18,
	0x00, 0x80, 0x2e, 0x18, 0x00, 0xfd, 0x2d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x2e,
	0x55, 0xf0, 0xc0, 0x2e, 0x21, 0x2e, 0x55, 0xf0, 0x80, 0x2e, 0x18, 0x00, 0xfd, 0x2d, 0xaa, 0x00, 0x05, 0x00, 0xaa,
	0x00, 0x05, 0x00, 0x40, 0x48, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x2e, 0x80, 0x00, 0x20, 0x26, 0x98, 0x2e, 0xef, 0x00, 0x10, 0x30, 0x21, 0x2e,
	0x59, 0xf0, 0x98, 0x2e, 0x38, 0x00, 0x98, 0x2e, 0x7f, 0x01, 0x98, 0x2e, 0x8e, 0x01, 0x00, 0x2e, 0x00, 0x2e, 0xd0,
	0x2e, 0x98, 0x2e, 0xce, 0x00, 0x01, 0x2e, 0x34, 0x00, 0x00, 0xb2, 0x0d, 0x2f, 0x00, 0x30, 0x21, 0x2e, 0x34, 0x00,
	0x01, 0x50, 0x98, 0x2e, 0x13, 0x01, 0x01, 0x50, 0x03, 0x52, 0x98, 0x2e, 0x00, 0xb0, 0x01, 0x50, 0x05, 0x52, 0x98,
	0x2e, 0x00, 0xb0, 0x98, 0x2e, 0x38, 0x00, 0xe6, 0x2d, 0x13, 0x52, 0x40, 0x30, 0x42, 0x40, 0x90, 0x0a, 0x42, 0x42,
	0x58, 0x84, 0x07, 0x52, 0xa1, 0x42, 0x71, 0x3c, 0x09, 0x56, 0x83, 0x42, 0xa9, 0x84, 0x83, 0x32, 0x84, 0x40, 0x61,
	0x08, 0x4b, 0x0a, 0x81, 0x42, 0x82, 0x82, 0x02, 0x3f, 0x43, 0x40, 0x9a, 0x08, 0x52, 0x42, 0x40, 0x42, 0x7e, 0x80,
	0x61, 0x30, 0x01, 0x42, 0x10, 0x50, 0x01, 0x2e, 0x40, 0xf0, 0x1a, 0x90, 0xfb, 0x7f, 0x20, 0x2f, 0x03, 0x30, 0x0d,
	0x50, 0x34, 0x33, 0x06, 0x30, 0x11, 0x52, 0x0b, 0x54, 0x55, 0x32, 0x1d, 0x1a, 0xe3, 0x22, 0x18, 0x1a, 0x0f, 0x58,
	0xe3, 0x22, 0x04, 0x30, 0xd5, 0x40, 0xb5, 0x0d, 0xe1, 0xbe, 0x6f, 0xbb, 0x80, 0x91, 0xa9, 0x0d, 0x01, 0x89, 0xb5,
	0x23, 0x10, 0xa1, 0xf7, 0x2f, 0xda, 0x0e, 0x34, 0x33, 0xeb, 0x2f, 0x01, 0x2e, 0x25, 0x00, 0x70, 0x1a, 0x00, 0x30,
	0x21, 0x30, 0x02, 0x2c, 0x08, 0x22, 0x30, 0x30, 0x00, 0xb2, 0x06, 0x2f, 0x21, 0x2e, 0x59, 0xf0, 0x98, 0x2e, 0x38,
	0x00, 0x00, 0x2e, 0x00, 0x2e, 0xd0, 0x2e, 0xfb, 0x6f, 0xf0, 0x5f, 0xb8, 0x2e, 0x1d, 0x50, 0x05, 0x2e, 0x00, 0xf0,
	0x17, 0x56, 0xd3, 0x0f, 0x01, 0x40, 0xf4, 0x33, 0xcc, 0x08, 0x0d, 0x2f, 0xf4, 0x30, 0x94, 0x08, 0xb9, 0x88, 0x02,
	0xa3, 0x04, 0x2f, 0x1b, 0x58, 0x4c, 0x0a, 0x87, 0xa2, 0x05, 0x2c, 0xcb, 0x22, 0x17, 0x54, 0x4a, 0x0a, 0xf2, 0x3b,
	0xca, 0x08, 0x3c, 0x80, 0x27, 0x2e, 0x59, 0xf0, 0x01, 0x40, 0x01, 0x42, 0xb8, 0x2e, 0x1a, 0x24, 0x26, 0x00, 0x80,
	0x2e, 0x58, 0x00, 0x00, 0x31, 0xc0, 0x2e, 0x21, 0x2e, 0xba, 0xf0, 0x12, 0x30, 0x12, 0x42, 0x02, 0x30, 0x12, 0x42,
	0x12, 0x42, 0x12, 0x42, 0x02, 0x42, 0x03, 0x80, 0x41, 0x84, 0x11, 0x42, 0x02, 0x42, 0xb8, 0x2e, 0x44, 0x47, 0x35,
	0x00, 0x46, 0x00, 0x4f, 0x00, 0xaf, 0x00, 0xff, 0x00, 0xff, 0xb7, 0x00, 0x02, 0x00, 0xb0, 0x05, 0x80, 0xb1, 0xf0,
	0x88, 0x00, 0x80, 0x00, 0x5e, 0xf0, 0xc0, 0x00, 0x59, 0xf0, 0x89, 0xf0, 0x38, 0x00, 0x40, 0x00, 0x42, 0x00, 0x60,
	0x50, 0x03, 0x2e, 0x45, 0x00, 0xe0, 0x7f, 0xf1, 0x7f, 0xdb, 0x7f, 0x30, 0x30, 0x15, 0x54, 0x0a, 0x1a, 0x28, 0x2f,
	0x1a, 0x25, 0x7a, 0x82, 0x00, 0x30, 0x43, 0x30, 0x32, 0x30, 0x05, 0x30, 0x04, 0x30, 0xf6, 0x6f, 0xf2, 0x09, 0xfc,
	0x13, 0xc2, 0xab, 0xb3, 0x09, 0xef, 0x23, 0x80, 0xb3, 0xe6, 0x6f, 0xb7, 0x01, 0x00, 0x2e, 0x8b, 0x41, 0x4b, 0x42,
	0x03, 0x2f, 0x46, 0x40, 0x86, 0x17, 0x81, 0x8d, 0x46, 0x42, 0x41, 0x8b, 0x23, 0xbd, 0xb3, 0xbd, 0x03, 0x89, 0x41,
	0x82, 0x07, 0x0c, 0x43, 0xa3, 0xe6, 0x2f, 0xe1, 0x6f, 0xa2, 0x6f, 0x52, 0x42, 0x00, 0x2e, 0xb2, 0x6f, 0x52, 0x42,
	0x00, 0x2e, 0xc2, 0x6f, 0x42, 0x42, 0x03, 0xb2, 0x06, 0x2f, 0x01, 0x2e, 0x59, 0xf0, 0x01, 0x32, 0x01, 0x0a, 0x21,
	0x2e, 0x59, 0xf0, 0x06, 0x2d, 0x01, 0x2e, 0x59, 0xf0, 0xf1, 0x3d, 0x01, 0x08, 0x21, 0x2e, 0x59, 0xf0, 0xdb, 0x6f,
	0xa0, 0x5f, 0xb8, 0x2e, 0x60, 0x50, 0xc3, 0x7f, 0xd4, 0x7f, 0xe7, 0x7f, 0xf6, 0x7f, 0xb2, 0x7f, 0xa5, 0x7f, 0x36,
	0x30, 0x07, 0x2e, 0x01, 0xf0, 0xbe, 0xbd, 0xbe, 0xbb, 0x1f, 0x58, 0x77, 0x05, 0x01, 0x56, 0x21, 0x54, 0x27, 0x41,
	0x06, 0x41, 0xf8, 0xbf, 0xbe, 0x0b, 0xb5, 0x11, 0xd6, 0x42, 0x03, 0x89, 0x5a, 0x0e, 0xf6, 0x2f, 0x12, 0x30, 0x25,
	0x2e, 0x34, 0x00, 0x02, 0x31, 0x25, 0x2e, 0xb8, 0xf0, 0xd4, 0x6f, 0xc3, 0x6f, 0xe7, 0x6f, 0xb2, 0x6f, 0xa5, 0x6f,
	0xf6, 0x6f, 0xa0, 0x5f, 0xc8, 0x2e, 0x10, 0x50, 0x23, 0x52, 0x03, 0x50, 0xfb, 0x7f, 0x98, 0x2e, 0xf3, 0x00, 0x03,
	0x52, 0x45, 0x82, 0x10, 0x30, 0x50, 0x42, 0x60, 0x30, 0xfb, 0x6f, 0xc0, 0x2e, 0x40, 0x42, 0xf0, 0x5f, 0x10, 0x50,
	0x25, 0x52, 0x05, 0x50, 0xfb, 0x7f, 0x98, 0x2e, 0xf3, 0x00, 0x05, 0x52, 0x45, 0x82, 0x00, 0x30, 0x50, 0x42, 0x70,
	0x30, 0xfb, 0x6f, 0xc0, 0x2e, 0x40, 0x42, 0xf0, 0x5f, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e,
	0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80,
	0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00,
	0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18,
	0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e,
	0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80,
	0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00,
	0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18,
	0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e,
	0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80,
	0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0x80, 0x2e, 0x18, 0x00, 0xfd, 0x2d, 0x46, 0x86,
	0x70, 0x50, 0xe5, 0x40, 0xc3, 0x88, 0x42, 0x84, 0x04, 0x41, 0xc3, 0x40, 0x06, 0x41, 0x6d, 0xbb, 0xc2, 0x7f, 0xf5,
	0x7f, 0x80, 0xb3, 0xe6, 0x7f, 0xd0, 0x7f, 0xb3, 0x7f, 0x12, 0x30, 0x5e, 0x2f, 0x31, 0x25, 0x55, 0x40, 0x41, 0x91,
	0xa1, 0x7f, 0x0f, 0x2f, 0x01, 0x30, 0xc1, 0x42, 0x00, 0x2e, 0xc2, 0x6f, 0x13, 0x40, 0x93, 0x42, 0x00, 0x2e, 0x13,
	0x40, 0x93, 0x42, 0x00, 0x2e, 0x00, 0x40, 0x80, 0x42, 0xbd, 0x80, 0xc0, 0x2e, 0x01, 0x42, 0x90, 0x5f, 0xc7, 0x86,
	0x01, 0x30, 0xc5, 0x40, 0xfb, 0x86, 0x45, 0x41, 0x04, 0x41, 0x43, 0xbe, 0xc3, 0xbb, 0xd5, 0xbe, 0x55, 0xba, 0x97,
	0x7f, 0x05, 0x30, 0xd1, 0x15, 0xf7, 0x09, 0xc0, 0xb3, 0x09, 0x2f, 0x06, 0x40, 0xc7, 0x40, 0xb7, 0x05, 0x07, 0x30,
	0x80, 0xa9, 0xfe, 0x05, 0xb7, 0x23, 0x74, 0x0f, 0x55, 0x23, 0xe6, 0x6f, 0x41, 0x82, 0x01, 0x80, 0xc1, 0x86, 0x43,
	0xa2, 0xec, 0x2f, 0xb0, 0x6f, 0xa4, 0x6f, 0x28, 0x1a, 0xd1, 0x6f, 0xc3, 0x6f, 0x02, 0x2f, 0x02, 0x30, 0x18, 0x2c,
	0x02, 0x43, 0x05, 0x41, 0x6a, 0x29, 0x96, 0x6f, 0x05, 0x43, 0x6e, 0x0e, 0x10, 0x2f, 0xf4, 0x6f, 0x00, 0xb3, 0x03,
	0x2f, 0x3f, 0x89, 0x94, 0x14, 0x25, 0x2e, 0x5e, 0xf0, 0x41, 0x25, 0x23, 0x25, 0x15, 0x41, 0x95, 0x42, 0x00, 0x2e,
	0x15, 0x41, 0x95, 0x42, 0x00, 0x2e, 0x04, 0x41, 0x84, 0x42, 0x00, 0x90, 0x09, 0x2f, 0x50, 0x40, 0xd0, 0x42, 0x00,
	0x2e, 0x50, 0x40, 0xd0, 0x42, 0x00, 0x2e, 0x40, 0x40, 0x02, 0x2c, 0xc0, 0x42, 0x42, 0x42, 0x90, 0x5f, 0xb8, 0x2e,
	0x00, 0x2e, 0x10, 0x24, 0x8a, 0x02, 0x11, 0x24, 0x00, 0x0c, 0x12, 0x24, 0x80, 0x2e, 0x13, 0x24, 0x18, 0x00, 0x12,
	0x42, 0x13, 0x42, 0x41, 0x1a, 0xfb, 0x2f, 0x10, 0x24, 0x50, 0x39, 0x11, 0x24, 0x21, 0x2e, 0x21, 0x2e, 0x10, 0x00,
	0x23, 0x2e, 0x11, 0x00, 0x80, 0x2e, 0x10, 0x00
};
#endif

#ifdef BMA400

/*
 * Read raw ACCEL data from the chip using SPI
 */
bool bma490l_getdata(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
			imu_cs(imu);
			SPI2_WriteRead(R_DATA_CMD, BMA400_DATA_LEN, imu->rbuf, BMA400_DATA_LEN);
			while (imu->run) {
			};
		}
		return imu->online;
	} else {
		return false;
	}
}
#else

/*
 * Read raw ACCEL data from the chip using SPI
 */
bool bma490l_getdata(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
			imu_cs(imu);
			SPI2_WriteRead(R_DATA_CMD, BMA490_DATA_LEN, imu->rbuf, BMA490_DATA_LEN);
			while (imu->run) {
			};
		}
		return imu->online;
	} else {
		return false;
	}
}
#endif

#ifdef BMA400

/*
 * see if we can get the correct ID response in rbuf
 * apply offset of received dummy data in the received buffer
 */
bool bma490l_getid(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
			imu_cs(imu);
			SPI2_WriteRead(R_ID_CMD, BMA490_REG_LEN, imu->rbuf, BMA490_REG_LEN + 1); // send two, receive 3 for the data 
			while (imu->run) {
			};
			snprintf(imu_buffer, max_buf, "  ID Chip Data %3x %3x", imu->rbuf[CHIP_ID_INDEX], imu->rbuf[CHIP_ID_DATA]);
			if (imu->rbuf[CHIP_ID_DATA] == BMA400_ID) {
				imu->online = true;
				imu->rbuf[CHIP_ID_DATA] = 0;
			} else {
				imu->online = false;
			}

		}
		return imu->online;
	} else {
		return false;
	}
}
#else

/*
 * see if we can get the correct ID response in rbuf
 */
bool bma490l_getid(void * imup)
{
	imu_cmd_t * imu = imup;

	if (imu) {
		if (!imu->run) {
			imu_cs(imu);
			SPI2_WriteRead(R_ID_CMD, sizeof(R_ID_CMD), imu->rbuf, sizeof(R_ID_CMD));
			delay_us(CHIP_ID_DELAY);
			if (imu->rbuf[CHIP_ID_DATA] == BMA490L_ID) {
				imu->online = true;
				imu->rbuf[CHIP_ID_DATA] = 0;
			} else {
				imu->online = false;
			}
		}
		return imu->online;
	} else {
		return false;
	}
}
#endif

/*
 * see if we can get the correct internal status from features file
 */
bool imu_getis(imu_cmd_t * imu)
{
	if (imu) {
		if (!imu->run) {
			imu_cs(imu);
			SPI2_WriteRead(R_IS_CMD, sizeof(R_IS_CMD), imu->rbuf, sizeof(R_IS_CMD));
			delay_us(CHIP_ID_DELAY);
			if (imu->rbuf[CHIP_ID_DATA] == 0x01) {
				imu->features = true;
				LED_GREEN_On();
				LED_RED_Off();
			} else {
				LED_RED_On();
				LED_GREEN_Off();
				imu->features = false;
				delay_us(750000);
			}
		}
		return imu->online;
	} else {
		return false;
	}
}

#ifdef XPRJ_nsensor
#ifndef BMA400
// dummy routine for host mode

void imu_set_reg(imu_cmd_t * imu, const uint8_t reg, const uint8_t data, const bool fast)
{

}
#endif
#endif

#ifdef BNO086
// dummy routine for bno086 mode

void imu_set_reg(imu_cmd_t * imu, const uint8_t reg, const uint8_t data, const bool fast)
{

}
#endif

#ifdef BMA400

/*
 * read or write IMU register without read data returned
 */
void imu_set_reg(imu_cmd_t * imu, const uint8_t reg, const uint8_t data, const bool fast)
{
	if (imu) {
		imu_cs(imu);
		imu->tbuf[0] = reg & WBIT; // clear MSB
		imu->tbuf[1] = data;
		SPI2_Write(imu->tbuf, BMA490_REG_LEN);
		if (!fast) {
			delay_us(100000); // 100ms for configuration delays
		}
		while (imu->run) {
		};
		imu_cs_disable(imu);
	}
}
#endif

#ifdef BMA400

/*
 * read IMU register read data returned in rbuf
 */
void imu_get_reg(imu_cmd_t * imu, const uint8_t reg, const bool fast)
{
	if (imu) {
		imu_cs(imu);
		imu->tbuf[0] = reg | RBIT;
		imu->tbuf[1] = 0;
		SPI2_WriteRead(imu->tbuf, BMA490_REG_LEN, imu->rbuf, BMA490_REG_LEN + 1);
		if (!fast) {
			delay_us(100000); // 100ms for configuration delays
		}
		while (imu->run) {
		};
		imu_cs_disable(imu);
	}
}
#endif

#ifndef BMA400
static void imu_gen_write(imu_cmd_t *, void*, size_t, const bool);

void imu_gen_write(imu_cmd_t * imu, void* pTransmitData, size_t txSize, const bool fast)
{
	if (imu && pTransmitData) {
		imu_cs(imu);
		SPI2_Write(pTransmitData, txSize);
		if (!fast) {
			delay_us(100000); // 100ms for configuration delays
		}
		while (imu->run) {
		};
		imu_cs_disable(imu);
	}
}
#endif

#ifdef BMA400

/*
 * toggle the chip CS to set BMA400 SPI mode and
 * configure operation modes for vibration sensor software
 */
void bma490l_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	static bool first = true;

	set_imu_bits(); // set 8 or 32-bit SPI transfers
	LED_GREEN_Off();
	LED_RED_On();

	if (imu) {
		if (first) {
			delay_us(5000); // power-up device delay
			imu_get_reg(imu, BMA400_CMD_DUMMY, false); // start SPI mode with a dummy read
			// use SPI interface on reboots so send a dummy ID command to clear errors
			bma490l_getid(imu);
		}

		imu_set_reg(imu, BMA400_REG_ACCEL_CONFIG_0, BMA400_NORM_MODE, false); // normal mode
		delay_us(1500);
		imu_set_reg(imu, BMA400_REG_ACCEL_CONFIG_1, BMA400_RANGE_MODE, false); // range=2g, osr=3 (high perf), acc_odr=100Hz:
		imu_set_reg(imu, BMA400_REG_ACCEL_CONFIG_2, BMA400_FILTER_MODE, false); // Use acc_filt2 (100Hz fixed) as data source
		imu_set_reg(imu, BMA400_REG_INT_CONF_0, BMA400_INT_CONF, false); // Enable data ready interrupt
		imu_set_reg(imu, BMA400_REG_INT_MAP, BMA400_INT_MAP, false); // Map data ready interrupt to INT1 pin -> ext2 interrupt on controller
		/*
		 * trigger ISR on IMU data update interrupts
		 */
		init_imu_int(imu);
		first = false;
	}
}
#else
static uint8_t dummy = 0x00;

/*
 * toggle the chip CS to set SPI mode
 */
void bma490l_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	static bool first = true;

	// set SPI MODE on BMA490L by reading ID register
	LED_GREEN_Off();
	LED_RED_On();
	if (imu) {
		if (first) {
			// use SPI interface on reboots
			imu_set_reg(imu, BMA490L_REG_NV_CONFIG, BMA490L_NV_DISABLE_I2C, false);
			// soft-reset IMU chip
			imu_set_reg(imu, BMA490L_REG_CMD, BMA490L_SOFT_RESET, false);
			imu_set_reg(imu, CHIP_ID | RBIT, dummy, false); // set read bit
			// PWR_CONF
			imu_set_reg(imu, BMA490L_REG_POWER_CONF, BMA490L_APS_OFF | BMA490L_FSW_ON, false);

			// INIT_CTRL,  init feature engine
			imu_set_reg(imu, BMA490L_REG_INIT_CTRL, BMA490L_INIT_START, false);
			/*
			 * burst write any/no motion features array, not working
			 */
			imu_gen_write(imu, (void *) bma490l_config_file, sizeof(bma490l_config_file), false);
			while (imu->run) {
			};
			// INIT_CTRL, enable sensor features
			imu_set_reg(imu, BMA490L_REG_INIT_CTRL, BMA490L_INIT_STOP, false);
			delay_us(200000);
			imu_getis(imu);

			// ACC_CONF
			imu_set_reg(imu, BMA490L_REG_ACCEL_CONFIG, ACCEL_CONFIG, false);
		}
		// ACC_RANGE
		imu_set_reg(imu, BMA490L_REG_ACCEL_RANGE, imu->acc_range, false);
		// INT_MAP_DATA
		imu_set_reg(imu, BMA490L_REG_INT_MAP_DATA, INT_MAP_DATA, false);
		// INT1_IO_CTRL
		imu_set_reg(imu, BMA490L_REG_INT1_IO_CTRL, INT1_IO_CTRL, false);
		// PWR_CTRL
		imu_set_reg(imu, BMA490L_REG_POWER_CTRL, REG_POWER_CTRL, false);
		/*
		 * trigger ISR on IMU data update interrupts
		 */
		init_imu_int(imu);
		first = false;
	}
}
#endif

/*
 * enable BMA490L CS and set flags
 */
bool imu_cs(imu_cmd_t * imu)
{
	if (imu) {
		switch (imu->cs) {
		case 0:
		default:
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
 * force BMA490L CS disabled
 */
void imu_cs_disable(imu_cmd_t * imu)
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
 * disables BMA490L CS and sets flags
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

#ifdef BMA400

void bma490_version(void)
{
	snprintf(imu_buffer, max_buf, " %s Driver Version  %s %s %s ", BMA400_ALIAS, BMA400_DRIVER, build_date, build_time);
}
#else

void bma490_version(void)
{
	snprintf(imu_buffer, max_buf, " %s Driver Version  %s %s %s ", BMA490_ALIAS, BMA490_DRIVER, build_date, build_time);
}
#endif

