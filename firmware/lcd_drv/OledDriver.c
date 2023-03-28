/************************************************************************/
/*										*/
/*	OledDriver.c	-- Graphics Driver Library for OLED Display		*/
/*										*/
/************************************************************************/
/*	Author: 	Gene Apperson						*/
/*	Copyright 2011, Digilent Inc.						*/
/************************************************************************/
/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
/************************************************************************/
/*  Module Description: 							*/
/*										*/
/*	This is part of the device driver software for the OLED bit mapped	*/
/*	display on the Digilent Basic I/O Shield. This module contains the	*/
/*	initialization functions and basic display control functions.		*/
/*										*/
/************************************************************************/
/*  Revision History:								*/
/*										*/
/*	04/29/2011(GeneA): Created						*/
/*	08/03/2011(GeneA): added functions to shut down the display and to	*/
/*		turn the display on and off.					*/
/*	01/04/2012(GeneA): Changed use of DelayMs to using standard delay	*/
/*		function. Removed delay.h					*/
/*	05/11/2017(FGB): convert XC32 and Harmony 2.0				*/
/*	08/01/2020(FGB): convert to Harmony 3.0 for pic32mk			*/
/*
 *	LCD graphic display routines
 *	machine dependant hardware routines should be here
 */
/************************************************************************/


/* ------------------------------------------------------------ */
/*				Include File Definitions	*/
/* ------------------------------------------------------------ */

#include "lcd_drv.h"

/* ------------------------------------------------------------ */
/*				Local Symbol Definitions						*/

/* ------------------------------------------------------------ */



/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */
volatile uint8_t disp_frame = lcd_frame;

/*
 * various fonts locations
 * types have been checked
 */
extern const uint8_t rgbOledFont0[];
extern uint8_t rgbOledFontUser[cbOledFontUser];
extern const uint8_t rgbFillPat[];

/* Coordinates of current pixel location on the display. The origin
 ** is at the upper left of the display. X increases to the right
 ** and y increases going down.
 */
int32_t xcoOledCur;
int32_t ycoOledCur;

uint8_t * pbOledCur; //address of byte corresponding to current location
int32_t bnOledCur; //bit number of bit corresponding to current location
uint8_t clrOledCur; //drawing color to use
uint8_t * pbOledPatCur; //current fill pattern
int32_t fOledCharUpdate;

int32_t dxcoOledFontCur;
int32_t dycoOledFontCur;

uint8_t * pbOledFontCur;
uint8_t * pbOledFontUser;

/* ------------------------------------------------------------ */
/*				Local Variables									*/
/* ------------------------------------------------------------ */

/* This array is the offscreen frame buffer used for rendering.
 ** It isn't possible to read back frome the OLED display device,
 ** so display data is rendered into this offscreen buffer and then
 ** copied to the display.
 *  must be in uncached memory for pic32 DMA so use __attribute__((coherent))
 * DMA0 SPI TX transfers DATA, CMD
 * DMA1 GLCD buffer transfers
 * DMA2 SPI TX transfers CMD, NOT USED
 */
#ifdef __32MK0512MCJ048__	// NO bank 2 for this CPU so memory is in bank 1
uint8_t __attribute__((address(BANK1), coherent)) rgbOledBmp0[cbOledDispMax]; // two display buffers for page flipping
uint8_t __attribute__((address(BANK1 + cbOledDispMax), coherent)) rgbOledBmp1[cbOledDispMax];
#ifdef USE_DMA
static uint8_t __attribute__((address(BANK1 - 8), coherent)) rgbOledBmp_blank[4] = {0x00, 0x00, 0x00, 0x00}; // 32-bit frame-buffer clearing variable
#endif
volatile uint8_t __attribute__((address(BANK1 - 16), coherent)) rgbOledBmp_page[5];
#endif

#ifdef __32MZ1025W104132__	// bank 2 for this CPU
uint8_t __attribute__((address(BANK2), coherent)) rgbOledBmp0[cbOledDispMax]; // two display buffers for page flipping
uint8_t __attribute__((address(BANK2 + cbOledDispMax), coherent)) rgbOledBmp1[cbOledDispMax];
#ifdef USE_DMA
static uint8_t __attribute__((address(BANK2 - 8), coherent)) rgbOledBmp_blank[4] = {0x00, 0x00, 0x00, 0x00}; // 32-bit frame-buffer clearing variable
#endif
volatile uint8_t __attribute__((address(BANK2 - 16), coherent)) rgbOledBmp_page[5];
#endif

static volatile DMA_RUN_STATE dstate = D_idle;

static const char *build_date = __DATE__, *build_time = __TIME__;

/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */

void OledHostInit(void);
void OledHostTerm(void);
void OledDevInit(void);
void OledDevTerm(void);
void OledDvrInit(void);
void OledPutBuffer(int32_t cb, uint8_t * rgbTx);

void CBDmaChannelHandler(DMAC_TRANSFER_EVENT, uintptr_t);
void SPI1DmaChannelHandler(DMAC_TRANSFER_EVENT, uintptr_t);
void SPI1DmaChannelHandler_State(DMAC_TRANSFER_EVENT, uintptr_t);
uint16_t SPI1_to_Buffer(uint8_t *, uint16_t, uint8_t *);

void RS_SetLow(void);
void RS_SetHigh(void);
void CSB_SetLow(void);
void CSB_SetHigh(void);
void SPI1_Exchange8bit(uint8_t);


/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */

/***	OledInit
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Initialize the OLED display subsystem.
 */

void OledInit(void)
{
	/* Init the memory variables used to control access to the
	 ** display.
	 */
	OledDvrInit();
	/*
	 * init DMA
	 */
#ifdef USE_DMA
#ifdef DMA_STATE_M
	DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, SPI1DmaChannelHandler_State, 0); // end of LCD buffer transfer interrupt function
#endif
	DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, CBDmaChannelHandler, 0); // end of buffer clear transfer interrupt function

	SPI1CONbits.STXISEL = DMA_GAP; // set to 0 for byte gaps
	SPI1CONbits.ENHBUF = true; // enable FIFO
#endif

	/* Clear the display.
	 */
	OledClear();
}

/* ------------------------------------------------------------ */

/***	OledTerm
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Shut down the OLED display.
 */

void OledTerm(void)
{
	/* Shut down the OLED display hardware.
	 */
	OledDevTerm();

	/* Release the PIC32 peripherals being used.
	 */
	OledHostTerm();
}

/* ------------------------------------------------------------ */

/***	OledHostInit
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Perform PIC32 device initialization to prepare for use
 **		of the OLED display.
 */

void OledHostInit(void)
{
	/*
	 * dogm does the init
	 */
}

/* ------------------------------------------------------------ */

/***	OledHostTerm
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Release processor resources used by the library
 */

void OledHostTerm(void)
{
	// does nothing, the display and SPI port remains active
}

/* ------------------------------------------------------------ */

/***	OledDvrInit
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Initialize the OLED software system
 */

void OledDvrInit(void)
{
	int32_t ib;

	/* Init the parameters for the default font
	 */
	dxcoOledFontCur = cbOledChar;
	dycoOledFontCur = 8;
	pbOledFontCur = (uint8_t *) rgbOledFont0;
	pbOledFontUser = rgbOledFontUser;

	for (ib = 0; ib < cbOledFontUser; ib++) {
		rgbOledFontUser[ib] = 0;
	}

	xchOledMax = ccolOledMax / dxcoOledFontCur;
	ychOledMax = crowOledMax / dycoOledFontCur;

	/* Set the default character cursor position.
	 */
	OledSetCursor(0, 0);

	/* Set the default foreground draw color and fill pattern
	 */
	clrOledCur = 0x01;
	pbOledPatCur = (uint8_t *) rgbFillPat;
	OledSetDrawMode(modOledSet);

	/* Default the character routines to manual
	 ** updates to the display.
	 */
	fOledCharUpdate = 0;
}

/* ------------------------------------------------------------ */

/***	OledDevInit
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Initialize the OLED display controller and turn the display on.
 */


void OledDevInit(void)
{
	/*
	 * handled by dogm-graphics
	 */
}

/* ------------------------------------------------------------ */

/***	OledDevTerm
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Shut down the OLED display hardware
 */

void OledDevTerm(void)
{
	/* Send the Display Off command.
	 * does nothing
	 */
}

/* ------------------------------------------------------------ */

/***	OledDisplayOn
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Turn the display on. This assumes that the display has
 **		already been powered on and initialized. All it does
 **		is send the display on command.
 */

void OledDisplayOn(void)
{
	/*
	 * does nothing
	 */
}

/* ------------------------------------------------------------ */

/***	OledDisplayOff
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Turn the display off. This does not power the display
 **		down. All it does is send the display off command.
 */

void OledDisplayOff(void)
{
	/*
	 * does nothing
	 */
}

/* ------------------------------------------------------------ */

/***	OledClear
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Clear the display. This clears the memory buffer and then
 **		updates the display.
 */

void OledClear(void)
{
	OledClearBuffer();
	OledUpdate();
}

/* ------------------------------------------------------------ */

/***	OledClearBuffer
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Clear the display memory buffer.
 */

/* just clears a output timing flag bit */
void CBDmaChannelHandler(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
	if (event == DMAC_TRANSFER_EVENT_COMPLETE) {
		//		DEBUGB0_Clear();
	}
}

void OledClearBuffer(void)
{
	uint8_t * pb;

	if (disp_frame) {
		pb = rgbOledBmp0;
	} else {
		pb = rgbOledBmp1;
	}

#ifdef USE_DMA
	/*
	 * DMAC_ChannelCallbackRegister in OledInit
	 */
	while (dstate != D_idle) {
	};
	wait_lcd_done();
	/* setup the source and destination parms */
	DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *) rgbOledBmp_blank, (size_t) 4, (const void*) pb, (size_t) cbOledDispMax, (size_t) cbOledDispMax);
	DCH1ECONSET = _DCH1ECON_CFORCE_MASK; // set CFORCE to 1 to start the transfer
#else
#endif
}

/* ------------------------------------------------------------ */

/***	OledUpdate
 **
 **	Parameters:
 **		none
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Update the OLED display with the contents of the memory buffer
 */

void OledUpdate(void)
{
#ifdef DMA_STATE_M
	wait_lcd_done();
	SPI1DmaChannelHandler_State(0, DMA_MAGIC); // set DMA state machine init mode to start transfers
	return;
#else
#endif
}

/*
 * start a GLCD update: Called in user code with contextHandle set to DMA_MAGIC for a background screen update,
 * during background transfers this function is used as the callback for DMA transfer complete events to
 * sequence commands and data to the GLCD via the SPI port using the dstate ENUM variable
 * dstate is set to 'D_idle' when the complete set of transfers is done.
 */
void SPI1DmaChannelHandler_State(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
	static int32_t ipag = 0; // buffer page number
	static uint8_t* pb; // buffer page address

	// back to mainline code, GLCD updates in background using DMA and interrupts
	LCD_UNSELECT();
	if (contextHandle == DMA_MAGIC) { // re-init state machine for next GLCD update
		dstate = D_init;
	}

	switch (dstate) {
	case D_init:
		ipag = 0;
		if (disp_frame) { // select flipper buffer
			pb = rgbOledBmp0;
		} else {
			pb = rgbOledBmp1;
		}
		/* FALLTHRU */
	case D_page: // send the page address commands via DMA
		LCD_SELECT(); // enable the GLCD chip for SPI transfers
		dstate = D_buffer;
		lcd_moveto_xy(ipag, 0); // calculate address data nibbles and store in rgbOledBmp_page array
		/*
		 * DMAC_ChannelCallbackRegister and SPI setup in OledInit
		 */
		LCD_CMD();
		DMAC_ChannelTransfer(DMAC_CHANNEL_0, (const void *) rgbOledBmp_page, (size_t) 4, (const void*) &SPI1BUF, (size_t) 1, (size_t) 1);
		break;
	case D_buffer: // send the GLCD buffer data via DMA
		ipag++;
		if (ipag <= cpagOledMax) {
			LCD_SELECT(); // enable the GLCD chip for SPI transfers
			dstate = D_page;
			LCD_DRAM();
			DMAC_ChannelTransfer(DMAC_CHANNEL_0, (const void *) pb, (size_t) ccolOledMax, (const void*) &SPI1BUF, (size_t) 1, (size_t) 1);
			pb += ccolOledMax;
		} else {
			dstate = D_idle;
			LCD_UNSELECT(); // all done with the GLCD
		}
		break;
	case D_idle:
	default:
		LCD_UNSELECT();
		break;
	}
}

/* ------------------------------------------------------------ */

/***	OledPutBuffer
 **
 **	Parameters:
 **		cb		- number of bytes to send
 **		rgbTx	- pointer to the buffer to send
 **
 **	Return Value:
 **		none
 **
 **	Errors:
 **		none
 **
 **	Description:
 **		Send the bytes specified in rgbTx to the slave OLED
 */


void OledPutBuffer(int32_t cb, uint8_t * rgbTx)
{
	SPI1_to_Buffer(rgbTx, cb, NULL);
}

void SPI1DmaChannelHandler(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
	if (event == DMAC_TRANSFER_EVENT_COMPLETE) {

		LCD_UNSELECT();
	}
}

uint16_t SPI1_to_Buffer(uint8_t *dataIn, uint16_t bufLen, uint8_t *dataOut)
{
	uint16_t bytesWritten = 0;

#ifdef USE_DMA
	wait_lcd_done();
	/*
	 * DMAC_ChannelCallbackRegister and SPI setup in OledInit
	 */
	LCD_SELECT();
	LCD_CMD();
	DMAC_ChannelTransfer(DMAC_CHANNEL_2, (const void *) rgbOledBmp_page, (size_t) 4, (const void*) &SPI1BUF, (size_t) 1, (size_t) 1);
	wait_lcd_done();
	bytesWritten = bufLen;
	LCD_SELECT();
	LCD_DRAM();
	if (bufLen != 0) {
		DMAC_ChannelTransfer(DMAC_CHANNEL_0, (const void *) dataIn, (size_t) bufLen, (const void*) &SPI1BUF, (size_t) 1, (size_t) 1);
	}
	return bytesWritten;
#else
#endif
}

void wait_lcd_done(void)
{
#ifdef USE_DMA
	while (DMAC_ChannelIsBusy(DMAC_CHANNEL_0)) {
	};
	while (DMAC_ChannelIsBusy(DMAC_CHANNEL_1)) {
	};
	while (DMAC_ChannelIsBusy(DMAC_CHANNEL_2)) {
	};
#endif
}

void lcd_version(void)
{
	printf("\r--- %s Driver Version  %s %s %s ---\r\n", LCD_ALIAS, LCD_DRIVER, build_date, build_time);
}