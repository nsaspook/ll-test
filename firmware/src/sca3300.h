/* 
 * File:   sca3300.h
 * Author: nsasp
 *
 * Created on August 31, 2022, 6:28 PM
 * SPI mode 0, 8MHz, 32-bit transfers
 */

#ifndef SCA3300_H
#define	SCA3300_H

#ifdef	__cplusplus
extern "C" {
#endif
	/*
	 * using code from Linux IIO driver for sca3300 https://elixir.bootlin.com/linux/latest/source/drivers/iio/accel/sca3300.c
	 */

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <math.h>
#include "definitions.h"                // SYS function prototypes
#include "imupic32mcj.h"
#include "imu.h"
#include "timers.h"
#include "canfd.h"

#define SCA3300_DRIVER	"V1.006" 

#define SCA3300_ALIAS	"SCx3300  "

	/* Device commands and response */
#define SCA3300_REG_WHOAMI	0x10
#define SCA3300_WHOAMI_ID	0x51
#define SCA3300_WHOAMI_ID_SCL	0xC1
#define SCA3300_WHOAMI_32B	0x40000091
#define SCA3300_SWRESET_32B	0xB4002098
#define SCA3300_RS_32B		0x180000E5
#define	SCA3300_ACC_X_32B	0x040000F7
#define	SCA3300_ACC_Y_32B	0x080000FD
#define	SCA3300_ACC_Z_32B	0x0C0000FB
#define SCA3300_TEMP_32B	0x140000EF
#define SCA3300_MODE1		0xB400001F	
#define SCA3300_MODE2		0xB4000102	
#define SCA3300_MODE3		0xB4000225
#define SCA3300_MODE4		0xB4000338
#define SCL3300_MODE1		0xB400001F
#define SCL3300_MODE2		0xB4000102
#define SCL3300_MODE3		0xB4000225
#define SCL3300_MODE4		0xB4000338
#define SCL3300_ANGLE		0xB0001F6F
#define SCL3300_E_FLAG1		0x1C0000E3
#define SCL3300_E_FLAG2		0x200000C1
#define	SCL3300_ANG_X_32B	0x240000C7
#define	SCL3300_ANG_Y_32B	0x280000CD
#define	SCL3300_ANG_Z_32B	0x2C0000CB
#define SCA3300_BANK0           0xFC000073
#define SCA3300_BANK1           0xFC00016E
#define SCA3300_SERIAL1         0x640000A7
#define SCA3300_SERIAL2         0x680000AD    

#define SCA3300_CHIP_ID_DELAY		10000	// ID command repeat delays in usec
#define SCA3300_CHIP_SWR_DELAY		10000	// chip software reset delay
#define SCA3300_CHIP_MODE_DELAY		100000	// chip G mode setup delay
#define SCA3300_CHIP_CS_DELAY		11	// CS high min duration between toggles
#define SCA3300_CHIP_BTYES_PER_SPI	4	// 32-bit transfers, 4 bytes

	/*
	 * function pointer templates
	 */
	void sca3300_set_spimode(void *);
	bool sca3300_getid(void *);
        bool sca3300_getserial(void *);
	bool sca3300_getdata(void *);
	void sca3300_version(void);

	enum sca3300_crc_check {
		SCA3300_REC = 0, // first item in the 32-bit receiver buffer
		SCA3300_TRM = 0, // first item in the 32-bit transmit buffer
	};

	uint8_t CalculateCRC(uint32_t);
	bool sca3300_imu_transfer(imu_cmd_t *, uint32_t);
	bool sca3300_check_crc(imu_cmd_t *, uint8_t);

#ifdef	__cplusplus
}
#endif

#endif	/* SCA3300_H */

