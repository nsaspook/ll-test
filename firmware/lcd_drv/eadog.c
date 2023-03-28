#include <xc.h>
#include "eadog.h"
#include <string.h>
#ifdef XPRJ_mcj
#include "../src/config/mcj/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "../src/config/mcj/peripheral/spi/spi_master/plib_spi1_master.h"
#include "../src/config/mcj/peripheral/gpio/plib_gpio.h"
#endif

#ifdef XPRJ_bma400
#include "../src/config/bma400/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "../src/config/bma400/peripheral/spi/spi_master/plib_spi1_master.h"
#include "../src/config/bma400/peripheral/gpio/plib_gpio.h"
#endif

#ifdef XPRJ_mcj_remote
#include "../src/config/mcj_remote/peripheral/spi/spi_master/plib_spi_master_common.h"
#include "../src/config/mcj_remote/peripheral/spi/spi_master/plib_spi1_master.h"
#include "../src/config/mcj_remote/peripheral/gpio/plib_gpio.h"
#endif


/*
 *	LCD character display routines
 *	machine dependant hardware routines should be here
 */

/* Global object to save SPI Exchange related data */
extern SPI_OBJECT spi1Obj;

#ifdef EDOGS
#define SPI1_CON_MSTEN                      (1 << _SPI1CON_MSTEN_POSITION)
#define SPI1_CON_CKP                        (1 << _SPI1CON_CKP_POSITION)
#define SPI1_CON_CKE                        (0 << _SPI1CON_CKE_POSITION)
#define SPI1_CON_MODE_32_MODE_16            (0 << _SPI1CON_MODE16_POSITION)
#define SPI1_CON_ENHBUF                     (1 << _SPI1CON_ENHBUF_POSITION)
#define SPI1_CON_MCLKSEL                    (0 << _SPI1CON_MCLKSEL_POSITION)
#define SPI1_CON_MSSEN                      (0 << _SPI1CON_MSSEN_POSITION)
#define SPI1_CON_SMP                        (0 << _SPI1CON_SMP_POSITION)
#endif

static void send_lcd_cmd_long(uint8_t); // for display init only
static void send_lcd_data(uint8_t);
static void send_lcd_cmd(uint8_t);
static volatile uint8_t NOPER = 0;

void SPI1_Initialize_internal(void)
{
	uint32_t rdata = 0U;

	/* Disable SPI1 Interrupts */
	IEC1CLR = 0x8;
	IEC1CLR = 0x10;
	IEC1CLR = 0x20;

	/* STOP and Reset the SPI */
	SPI1CON = 0;

	/* Clear the Receiver buffer */
	rdata = SPI1BUF;
	rdata = rdata;

	/* Clear SPI1 Interrupt flags */
	IFS1CLR = 0x8;
	IFS1CLR = 0x10;
	IFS1CLR = 0x20;

	/* BAUD Rate register Setup */
	SPI1BRG = 1;

	/* CLear the Overflow */
	SPI1STATCLR = _SPI1STAT_SPIROV_MASK;

	/*
	MSTEN = 1
	CKP = 0
	CKE = 1
	MODE<32,16> = 0
	ENHBUF = 1
	MSSEN = 0
	MCLKSEL = 0
	 */
	SPI1CONSET = (SPI1_CON_MSSEN | SPI1_CON_MCLKSEL | SPI1_CON_ENHBUF | SPI1_CON_MODE_32_MODE_16 | SPI1_CON_CKE | SPI1_CON_CKP | SPI1_CON_MSTEN | SPI1_CON_SMP);

	/* Enable transmit interrupt when transmit buffer is completely empty (STXISEL = '01') */
	/* Enable receive interrupt when the receive buffer is not empty (SRXISEL = '01') */
	SPI1CONSET = 0x00000005;

	/* Initialize global variables */
	spi1Obj.transferIsBusy = false;
	spi1Obj.callback = NULL;

	/* Enable SPI1 */
	SPI1CONSET = _SPI1CON_ON_MASK;
}

void ReSet_SetLow(void)
{
	wait_lcd_done();
	EXT_SW1_Clear();
};

void ReSet_SetHigh(void)
{
	wait_lcd_done();
	EXT_SW1_Set();
};

void RS_SetLow(void)
{
	wait_lcd_done();
	DIS_MODE_Clear();
};

void RS_SetHigh(void)
{
	wait_lcd_done();
	DIS_MODE_Set();
};

void CSB_SetLow(void)
{
	wait_lcd_done();
	DIS_CS_Clear();
};

void CSB_SetHigh(void)
{
	wait_lcd_done();
	DIS_CS_Set();
};

void SPI1_Exchange8bit(uint8_t data)
{
	wait_lcd_done();
	SPI1_Write(&data, 1);
};

void SPI1_ExchangeBuffer(uint8_t *data, uint16_t len)
{
	wait_lcd_done();
	SPI1_Write(data, len);
};

void wdtdelay(const uint32_t delay)
{
	static uint32_t dcount;

	for (dcount = 0; dcount <= delay; dcount++) { // delay a bit
		NOPER++;
	};
}

/*
 * Init the EA DOGM163 in 8-bit serial mode
 */
void init_display(void)
{
	CSB_SetHigh();
	wdtdelay(IS_DELAYPOWERUP); // > 400ms power up delay
	send_lcd_cmd(0x39);
	send_lcd_cmd(0x15);
	send_lcd_cmd(0x55);
	send_lcd_cmd(0x6d);
	send_lcd_cmd(0x70); // contrast last 4 bits
	send_lcd_cmd_long(0x38); // follower control
	send_lcd_cmd(0x0f);
	send_lcd_cmd_long(0x01); // clear
	send_lcd_cmd(0x02);
	send_lcd_cmd(0x06);
	wdtdelay(IS_DELAYLONG);
}

/*
 * add short spi delay (default)
 */
static void send_lcd_data(const uint8_t data)
{
	RS_SetHigh();
	CSB_SetLow();
	wdtdelay(IS_DELAYSHORT);
	SPI1_Exchange8bit(data);
}

/*
 * add inst spi delay
 */
static void send_lcd_cmd(const uint8_t cmd)
{
	RS_SetLow();
	CSB_SetLow();
	wdtdelay(IS_DELAYMED);
	SPI1_Exchange8bit(cmd);
	wdtdelay(IS_DELAYMED);
	RS_SetHigh();
}

/*
 * add clear/home spi delay
 */
static void send_lcd_cmd_long(const uint8_t cmd)
{
	RS_SetLow();
	CSB_SetLow();
	SPI1_Exchange8bit(cmd);
	wdtdelay(IS_DELAYLONG);
	RS_SetHigh();
}

void eaDogM_WriteChr(const int8_t value)
{
	send_lcd_data((uint8_t) value);
}

void eaDogM_WriteCommand(const uint8_t cmd)
{
	send_lcd_cmd(cmd);
}

void eaDogM_SetPos(const uint8_t r, const uint8_t c)
{
	uint8_t cmdPos;
	cmdPos = (uint8_t) EADOGM_CMD_DDRAM_ADDR + (uint8_t) ((uint8_t) r * (uint8_t) EADOGM_COLSPAN) + (uint8_t) c;
	eaDogM_WriteCommand(cmdPos);
}

void eaDogM_ClearRow(const uint8_t r)
{
	uint8_t i;
	eaDogM_SetPos(r, 0);
	for (i = 0; i < EADOGM_COLSPAN; i++) {
		eaDogM_WriteChr(' ');
	}
}

void eaDogM_WriteString(char *strPtr)
{
	uint8_t i = strlen(strPtr);

	RS_SetHigh();
	CSB_SetLow();

	if (i > max_strlen) {
		strPtr[max_strlen] = 0; // buffer overflow check
		i = max_strlen;
	}

	SPI1_Write(strPtr, i); // use interrupt mode so we don't wait
}

void eaDogM_WriteStringAtPos(const uint8_t r, const uint8_t c, char *strPtr)
{
#ifdef EDOGS
	OledSetCursor(c, r);
	OledPutString(strPtr);
#else
	send_lcd_cmd((EADOGM_CMD_DDRAM_ADDR + (r * EADOGM_COLSPAN) + c));
	eaDogM_WriteString(strPtr);
#endif
}

void eaDogM_WriteIntAtPos(uint8_t r, uint8_t c, uint8_t i)
{
	eaDogM_WriteCommand((EADOGM_CMD_DDRAM_ADDR + (r * EADOGM_COLSPAN) + c));

	eaDogM_WriteChr(i / 10 + '0');
	eaDogM_WriteChr(i % 10 + '0');

}

// this writes a byte to the internal CGRAM (v2.02)
// format for ndx: 00CCCRRR = CCC = character 0 to 7, RRR = row 0 to 7

void eaDogM_WriteByteToCGRAM(uint8_t ndx, uint8_t data)
{
	uint8_t cmd;

	cmd = ndx & 0b00111111; // mask off upper to bits
	cmd = cmd | EADOGM_CMD_CGRAM_ADDR; // set bit cmd bits

	eaDogM_WriteCommand(cmd);
	eaDogM_WriteChr(data);

	// this is done to make sure we are back in data mode
	eaDogM_SetPos(0, 0);
}