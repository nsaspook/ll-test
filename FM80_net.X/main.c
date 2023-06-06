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

#include "mxcmd.h"

#define PACE	31000	// commands delay in count units
#define CMD_LEN	8
#define REC_LEN 5
#define REC_STATUS_LEN	16

enum state_type {
	state_init,
	state_status,
	state_panel,
	state_batteryv,
	state_batterya,
	state_watts,
	state_misc,
	state_mx_status,
	state_last,
};

uint16_t abuf[FM_BUFFER];
uint16_t volt_fract;
uint16_t volt_whole, panel_watts, cc_mode;
enum state_type state = state_init;
uint16_t pacing = 0, rx_count = 0, flush;
volatile bool mx80_online = true;

mx_status_packed_t *status_packed = (void *) abuf;
/*
 * show fixed point fractions
 */
void volt_f(uint16_t);

/*
 * MX80 send/recv functions
 */
void send_mx_cmd(const uint16_t *);
void rec_mx_cmd(void (* DataHandler)(void), uint8_t);

/*
 * callbacks to handle MX80 register data
 */
void state_init_cb(void);
void state_status_cb(void);
void state_panelv_cb(void);
void state_batteryv_cb(void);
void state_batterya_cb(void);
void state_watts_cb(void);
void state_misc_cb(void);
void state_mx_status_cb(void);

/*
 * Main application
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
	TMR2_SetInterruptHandler(tensec_io);
	TMR2_StartTimer();

	while (true) {
		// Add your application code
		switch (state) {
		case state_init:
			send_mx_cmd(cmd_id);
			rec_mx_cmd(state_init_cb, REC_LEN);
			break;
		case state_status:
			send_mx_cmd(cmd_status);
			rec_mx_cmd(state_status_cb, REC_LEN);
			break;
		case state_panel:
			send_mx_cmd(cmd_panelv);
			rec_mx_cmd(state_panelv_cb, REC_LEN);
			break;
		case state_batteryv:
			send_mx_cmd(cmd_batteryv);
			rec_mx_cmd(state_batteryv_cb, REC_LEN);
			break;
		case state_batterya:
			send_mx_cmd(cmd_batterya);
			rec_mx_cmd(state_batterya_cb, REC_LEN);
			break;
		case state_watts:
			send_mx_cmd(cmd_watts);
			rec_mx_cmd(state_watts_cb, REC_LEN);
			break;
		case state_mx_status: // wait for ten second flag in this state for logging 
			send_mx_cmd(cmd_mx_status);
			rec_mx_cmd(state_mx_status_cb, REC_STATUS_LEN);
			break;
		case state_misc:
			send_mx_cmd(cmd_misc);
			rec_mx_cmd(state_misc_cb, REC_LEN);
			break;
		default:
			send_mx_cmd(cmd_id);
			rec_mx_cmd(state_init_cb, REC_LEN);
			break;
		}
	}
}

/*
 * display  div 10 integer to fraction without FP
 * %d.%01d  volt_whole, volt_fract
 */
void volt_f(uint16_t voltage)
{
	volt_fract = (uint16_t) abs(voltage % 10);
	volt_whole = voltage / 10;
}

void send_mx_cmd(const uint16_t * cmd)
{
	if (FM_tx_empty()) {
		if (pacing++ > PACE) {
			FM_tx(cmd, CMD_LEN); // send 8 9-bits command data stream
			pacing = 0;
		}
	}
}

/*
 * process received data in abuf with callbacks
 */
void rec_mx_cmd(void (* DataHandler)(void), uint8_t rec_len)
{
	if (FM_rx_ready()) {
		if (FM_rx_count() >= rec_len) {
			FM_rx(abuf);
			DataHandler(); // execute callback
		}
	}
}

void state_init_cb(void)
{
	if (abuf[2] == 0x03) {
		printf("\r\n\r\n%5d %3x %3x %3x %3x %3x   INIT: Found MX80 online\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4]);
		mx80_online = true;
	} else {
		printf("\r\n\r\n%5d %3x %3x %3x %3x %3x   INIT: MX80 Not Found online\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4]);
		mx80_online = false;
	}
	state = state_status;
}

void state_status_cb(void)
{
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x STATUS: MX80 %s mode\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], state_name[abuf[2]]);
#endif
	if (abuf[2] != STATUS_SLEEPING) {
		state = state_watts;
	} else {
		state = state_mx_status;
	}
	cc_mode = abuf[2];
}

void state_panelv_cb(void)
{
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Panel Voltage %iVDC\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], (abuf[2] + (abuf[1] << 8)));
#endif
	state = state_batteryv;
}

void state_batteryv_cb(void)
{
	volt_f((abuf[2] + (abuf[1] << 8)));
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Battery Voltage %d.%01dVDC\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], volt_whole, volt_fract);
#endif
	state = state_batterya;
}

void state_batterya_cb(void)
{
	volt_f((abuf[2] + (abuf[1] << 8)));
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Battery Amps %dADC\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], abuf[2] - 128);
#endif
	state = state_watts;
}

void state_watts_cb(void)
{
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Panel Watts %iW\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], (abuf[2] + (abuf[1] << 8)));
#endif
	panel_watts = (abuf[2] + (abuf[1] << 8));
	state = state_mx_status;
}

void state_mx_status_cb(void)
{
	uint16_t vf, vw;

	volt_f((abuf[11] + (abuf[10] << 8)));
	vw = volt_whole;
	vf = volt_fract;
	volt_f((abuf[13] + (abuf[12] << 8)));
	if ((abuf[1] &0x0f) > 9) { // check for whole Amp
		abuf[2]++; // add extra Amp for fractional overflow.
		abuf[1] = (abuf[1]&0x0f) - 10;
	}
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x  SDATA: MX80 Data mode %3x %3x %3x %3x %3x %3x %3x %3x %3x\r\n",
		rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], abuf[5], abuf[6], abuf[7], abuf[8], abuf[9], abuf[10], abuf[11], abuf[12], abuf[13]);
#endif
	if (ten_sec_flag) {
		ten_sec_flag = false;
		if (mx80_online) {
			/*
			 * log CSV values to the serial port for data storage and processing
			 */
			printf("^^^,%d.%01d,%d.%01d,%d,%d.%01d,%d,%d,%d\r\n", abuf[3] - 128, abuf[1]&0x0f, vw, vf, abuf[2] - 128, volt_whole, volt_fract, panel_watts, cc_mode, rx_count++);
		}
	}
	state = state_misc;
}

/*
 * testing online status while waiting for 10 second flag callback
 */
void state_misc_cb(void)
{
	if (abuf[2] == 0x03) {
		mx80_online = true;
	} else {
		mx80_online = false;
	}
	if (!ten_sec_flag) {
		state = state_misc;
	} else {
		state = state_status;
	}
}
/**
 End of File
 */