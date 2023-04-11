/* 
 * File:   imupic32mcj.h
 * Author: nsasp
 *
 * Created on August 31, 2022, 6:40 PM
 */

#ifndef IMUPIC32MCJ_H
#define IMUPIC32MCJ_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <math.h>
#include "definitions.h"                // SYS function prototypes
#include "imu.h"
#include "timers.h"

	/*
	 * Board hardware configurations
	 * and hardware specific functions
	 */

	/*
	 * what type of Ethernet TCP adapter are we using
	 */
	//#define USR_TCP
#define WAVESHARE

	/*
	 * what IMU chip are we using
	 */
#define SCA3300 // this includes the SCL3300 device, SPI2 32-bit transfers
	//#define BMA400 // Bosch devices, SPI2 8-bit transfers

#ifdef SCA3300
#define	SPI2_32BIT
#undef BMA400
#endif


#ifdef BMA400 // vibration sensor used
#define BMA490L // this device is not longer used but functions are needed
#undef SCA3300
#endif

#define IMU_CS 0
#define LCD_CS 0

#define BMA_LOG_TIMEOUT 150
#define SCA_LOG_TIMEOUT 8 // 3

	//#define SHOW_LOG
#define SHOW_LCD
#define SHOW_VG
#define FFT_MIX false

#define FBUFFER_SIZE	256
#define max_buf		FBUFFER_SIZE-1
#define RBUFFER_SIZE	FBUFFER_SIZE

#ifndef  LED_GREEN_On
#define LED_GREEN_On() LED_GREEN_Set()
#define LED_GREEN_Off() LED_GREEN_Clear()
#endif
#ifndef  LED_RED_On
#define LED_RED_On() LED_RED_Set()
#define LED_RED_Off() LED_RED_Clear()
#endif 

	/*
	 * setup external interrupt #2 for IMU BMA4x0 data update interrupt trigger output
	 */
	void init_imu_int(const imu_cmd_t *);
	/*
	 * user callback function per BMA490L data interrupt
	 */
	void update_imu_int1(uint32_t, uintptr_t);
	void delay_us(uint32_t);
	/*
	 * start core-timer
	 * ms tick-timer
	 * set cpu serial ID numbers
	 */
	void start_tick(void);
	/*
	 * configure the SPI port bit size for data transfers
	 */
	uint8_t set_imu_bits(void);
	void canfd_set_filter(uint32_t, uint32_t);
	void clear_can_errors(void);

	extern uint32_t board_serial_id, cpu_serial_id;

#ifdef __cplusplus
}
#endif

#endif /* IMUPIC32MCJ_H */

