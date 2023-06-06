#include "mxcmd.h"

static volatile uint8_t data = 0x00, dcount = 0, dstart = 0, rdstart = 0;
static volatile uint16_t tbuf[FM_BUFFER], rbuf[FM_BUFFER];
static uint16_t *p_tbuf = (uint16_t*) tbuf, *p_rbuf = (uint16_t*) rbuf;
volatile bool ten_sec_flag = false;
static volatile uint8_t pace = 0; // the charge controller doesn't like back to back bytes

void FM_restart(void)
{
	data = U1RXB;
	dcount = 0;
	dstart = 0;
	rdstart = 0;
	ten_sec_flag = false;
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
uint8_t FM_tx(const uint16_t * data, uint8_t count)
{
	if (dcount == 0) {
		memcpy((void *) tbuf, (const void *) data, (size_t) (count << 2)); // copy 16-bit values
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
	MISC_SetHigh(); // serial CPU usage signal

	if (pace++ > BUFFER_SPACING) {
		if (dcount-- > 0) {
			if (tbuf[dstart] > 0xff) { // Check for bit-9
				U1P1L = (uint8_t) tbuf[dstart]; // send with bit-9 high, start of packet
			} else {
				UART1_Write((uint8_t) tbuf[dstart]); // send with bit-9 low
			}
			dstart++;
		} else {
			dstart = 0;
			dcount = 0;
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
		if (U1ERRIRbits.PERIF) {
			rdstart = 0; // restart receive buffer when we see a 9-th bit high
			rbuf[rdstart] = 0x0100; // start of packet
		} else {
			rbuf[rdstart] = 0x00;
		}
		rbuf[rdstart] += U1RXB;
		rdstart++;
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
	//	RLED_Toggle();
	MLED_SetLow();
}

void tensec_io(void)
{
	RLED_Toggle();
	MLED_SetLow();
	ten_sec_flag = true;
}
