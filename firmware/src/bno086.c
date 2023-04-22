#include "bno086.h"

static const char *build_date = __DATE__, *build_time = __TIME__;

sh2_Quaternion_t fusion_q;

void bno086_set_spimode(void * imup)
{
	imu_cmd_t * imu = imup;
	static bool first = true;

	// set SPI MODE on BMA490L by reading ID register
	LED_GREEN_Off();
	LED_RED_On();
	if (imu) {
		if (first) {
		}
		/*
		 * trigger ISR on IMU data update interrupts
		 */
		init_imu_int(imu);
		first = false;
	}
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
	snprintf(imu_buffer, max_buf, " %s Driver Version  %s %s %s ", BNO086_ALIAS, BNO086_DRIVER, build_date, build_time);
}
