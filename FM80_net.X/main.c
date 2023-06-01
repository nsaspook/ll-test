/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
	Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
	Device            :  PIC18F14Q41
	Driver Version    :  2.00
 */

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */
#pragma warning disable 520
#pragma warning disable 1498

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"

#define	FM_BUFFER	32

volatile uint8_t data = 0x00, dcount = 0, dstart = 0, rdstart = 0;
volatile uint16_t tbuf[FM_BUFFER] = {0x100, 0x000, 0x1ff, 0x0ff}, rbuf[FM_BUFFER];
uint16_t *p_tbuf = (uint16_t*) tbuf, *p_rbuf = (uint16_t*) rbuf, abuf[FM_BUFFER];

void onesec_io(void);
void FM_io(void);
uint8_t FM_tx(uint8_t);
bool FM_tx_empty(void);
uint8_t FM_rx(uint16_t *);
bool FM_rx_ready(void);
uint8_t FM_rx_count(void);

/*
			 Main application
 */
void main(void)
{
	// Initialize the device
	SYSTEM_Initialize();

	// If using interrupts in PIC18 High/Low Priority Mode you need to enable the Global High and Low Interrupts
	// If using interrupts in PIC Mid-Range Compatibility Mode you need to enable the Global Interrupts
	// Use the following macros to:

	// Enable high priority global interrupts
	INTERRUPT_GlobalInterruptHighEnable();

	// Enable low priority global interrupts.
	INTERRUPT_GlobalInterruptLowEnable();

	// Disable high priority global interrupts
	//INTERRUPT_GlobalInterruptHighDisable();

	// Disable low priority global interrupts.
	//INTERRUPT_GlobalInterruptLowDisable();

	TMR4_SetInterruptHandler(FM_io);
	TMR4_StartTimer();
	TMR0_SetInterruptHandler(onesec_io);
	TMR0_StartTimer();

	while (true) {
		// Add your application code
		if (FM_tx_empty()) {
			FM_tx(4);
		}

		if (FM_rx_ready()) {
			if (FM_rx_count() > 16) {
				FM_rx(abuf);
			}
		}
	}
}

/*
 * Check for TX transmission
 */
bool FM_tx_empty(void)
{
	if (dcount == 0) {
		return true;
	} else {
		return false;
	}
}

/*
 * after the tbuf has been loaded start the TX transfer
 */
uint8_t FM_tx(uint8_t count)
{
	if (dcount == 0) {
		dstart = 0;
		dcount = count;
	}
	return dstart;
}

/*
 * serial I/O ISR, TMR4 500us I/O sample rate
 * polls the required UART registers for 9-bit send and receive into 16-bit arrays
 */
void FM_io(void)
{
	static uint8_t pace = 0;

	MISC_SetHigh(); // serial CPU usage signal

	if (pace++ > 3) {
		if (dcount-- > 0) {
			if (tbuf[dstart] > 0xff) { // Check for bit-9
				U1P1L = (uint8_t) tbuf[dstart]; // send with bit-9 high
			} else {
				UART1_Write((uint8_t) tbuf[dstart]); // send with bit-9 low
			}
			dstart++;
		} else {
			dstart = 0;
		}
		pace = 0;
	}

	/*
	 * handle framing errors
	 */
	if (U1ERRIRbits.RXFOIF) {
		rbuf[0] = U1RXB; // read bad data to clear error
		U1ERRIRbits.RXFOIF = 0;
		rdstart = 0; // reset buffer to start
	}

	/*
	 * read serial data if polled interrupt flag is set
	 */
	if (PIR4bits.U1RXIF) {
		if (U1ERRIRbits.FERIF) {
		}

		if (rdstart > FM_BUFFER - 1) { // overload buffer index
			rdstart = 0; // reset buffer to start
			MLED_SetHigh();
		}
		rbuf[rdstart++] = U1RXB;
	}
	MISC_SetLow();
}

uint8_t FM_rx(uint16_t * data)
{
	uint8_t count;

	RELAY_SetHigh();
	INTERRUPT_GlobalInterruptHighDisable();
	count = rdstart;
	memcpy(data, (const void *) rbuf, (size_t) (count << 2)); // copy 16-bit values
	rdstart = 0;
	INTERRUPT_GlobalInterruptHighEnable();
	RELAY_SetLow();
	return count;
}

bool FM_rx_ready(void)
{
	if (rdstart == 0) {
		return false;
	} else {
		return true;
	}
}

uint8_t FM_rx_count(void)
{
	uint8_t count;

	INTERRUPT_GlobalInterruptHighDisable();
	count = rdstart;
	INTERRUPT_GlobalInterruptHighEnable();
	return count;
}

void onesec_io(void)
{
	RLED_Toggle();
	MLED_SetLow();
}
/**
 End of File
 */