#include "bno086.h"

static const char *build_date = __DATE__, *build_time = __TIME__;

void bno086_version(void)
{
	snprintf(imu_buffer, max_buf, " %s Driver Version  %s %s %s ", BNO086_ALIAS, BNO086_DRIVER, build_date, build_time);
}
