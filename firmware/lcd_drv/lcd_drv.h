/*
 * File:   lcd_drv.h
 * Author: root
 *
 * Created on October 12, 2021, 7:01 PM
 */

#ifndef LCD_DRV_H
#define	LCD_DRV_H

#ifdef	__cplusplus
extern "C" {
#endif

#define DMA_MAGIC	1957
#define USE_DMA // use DMA spi driver

#ifdef USE_DMA
#define	DMA_GAP		1	// set to 0 for SPI byte gaps in DMA transmissions
#define DMA_STATE_M
#endif

#define EDOGS

	/*
	 * DMA driver for DOGXL240 @15MHz SPI clock
	 * 2021 XC32 and H3
	 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes
#ifdef __32MK0512MCJ048__X
#include "../src/config/mcj/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "../src/config/mcj/peripheral/gpio/plib_gpio.h"
#include "../src/config/mcj/peripheral/dmac/plib_dmac.h"
#include "../src/config/mcj/peripheral/coretimer/plib_coretimer.h"
#endif
#ifdef __32MZ1025W104132__X
#include "../src/config/default/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "../src/config/default/peripheral/gpio/plib_gpio.h"ww
#include "../src/config/default/peripheral/dmac/plib_dmac.h"
#include "../src/config/default/peripheral/coretimer/plib_coretimer.h"
#endif
#include "display_type.h"
#include "eadog.h"
#include "dogm-graphic.h"
#include "OledDriver.h"
#include "OledChar.h"
#include "OledGrph.h"

#include "device.h"

#define BMP_DELAY	1000000	// image display delay counts
#define bmp_x		60		// screen positions of image
#define bmp_y		24
#define bmp_size	100		// x,y size of image
#define	BANK1		0xA000A000	// bank 1 frame buffer memory address
#define	BANK2		0x80030000	// bank 2 frame buffer memory address

#define LCD_DRIVER	"V1.003" 
#define LCD_ALIAS	"EADOG LCD"

	typedef enum {
		D_INIT,
		D_MISC,
	} LCD_DVR_STATE;

#define lcd_frame	0	// frame buffer 1 or 0

	void init_lcd_drv(LCD_DVR_STATE init_type);
	void lcd_version(void);

	extern const uint8_t foo_map[]; // image in flash array

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_DRV_H */

