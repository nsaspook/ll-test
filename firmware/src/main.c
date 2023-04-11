
/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*
 * BMA490L, BMA400 and SCA3300 high-performance 16-bit digital triaxial acceleration sensor DEMO
 * http://download.mikroe.com/documents/datasheets/BMA490L%20Datasheet.pdf
 * for logging XYZ force values @ 115200 via a serial comm port
 * Original test configuration: 
 * Accel 15 Click: https://www.mikroe.com/accel-15-click
 * PIC32 Wi-Fi MCU module eval board: https://www.microchip.com/en-us/development-tool/EV12F11A
 * USB to TTL serial adapter: http://www.dsdtech-global.com/2017/07/dsd-tech-usb-to-ttl-serial-converter.html
 * 
 * Custom board PIC32MK0512MCJ048
 * PCB IMU
 * https://www.murata.com/-/media/webrenewal/products/sensor/pdf/datasheet/datasheet_sca3300-d01.ashx?la=en-us&cvid=20190620010315610400
 * PCB CPU
 * https://ww1.microchip.com/downloads/aemDocuments/documents/MCU32/ProductDocuments/DataSheets/PIC32MK-General-Purpose-and-Motor-Control-With-CAN-FD-Family-DataSheet-DS60001570D.pdf
 * 
 * upgraded to 460800 uart speed and ttl serial to Ethernet TDP server module
 * 32ms IMU updates when in sensor mode sent over 4 CAN packet durations of 3ms 0.5ms 0.5ms 1ms
 * The Max sensor count is ~6 because of the serial ttl speed bottleneck
 */

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <string.h>

#include "imupic32mcj.h"
#include "bma490l.h"
#include "sca3300.h"
#include "timers.h"
#include "../../firmware/lcd_drv/lcd_drv.h"
#include "gfx.h"
#include "buzzer.h"

#ifdef __32MK0512MCJ048__
#include "canfd.h"
#ifdef XPRJ_mcj
#include "config/mcj/peripheral/qei/plib_qei2.h"
#endif

#ifdef XPRJ_bma400
//#include "config/bma400/peripheral/qei/plib_qei2.h"
#endif

#ifdef XPRJ_mcj_remote
#include "config/mcj_remote/peripheral/qei/plib_qei2.h"
#endif

#endif
#include "pid.h"
#include "do_fft.h"
#include "host.h"

#ifdef BMA490L
/*
 * BMA490L instance
 */
imu_cmd_t imu0 = {
	.id = CAN_IMU_INFO,
	.tbuf[0] = CHIP_ID | RBIT,
	.online = false,
	.device = IMU_BMA490L, // device type
	.cs = IMU_CS, // chip select number
	.run = false,
	.log_timeout = BMA_LOG_TIMEOUT,
	.update = true,
	.features = false,
	.spi_bytes = 1,
	.op.info_ptr = &bma490_version,
	.op.imu_set_spimode = &bma490l_set_spimode,
	.op.imu_getid = &bma490l_getid,
	.op.imu_getdata = &bma490l_getdata,
	.acc_range = range_2g,
	.locked = true,
	.warn = false,
	.down = false,
};
#endif

#ifdef SCA3300
/*
 * SCA3300-D01 instance
 */
imu_cmd_t imu0 = {
	.id = CAN_IMU_INFO,
	.tbuf32[SCA3300_TRM] = SCA3300_SWRESET_32B,
	.online = false,
	.device = IMU_SCA3300, // device type
	.cs = IMU_CS, // chip select number
	.run = false,
	.crc_error = false,
	.log_timeout = SCA_LOG_TIMEOUT,
	.update = true,
	.features = false,
	.spi_bytes = 4,
	.op.info_ptr = &sca3300_version,
	.op.imu_set_spimode = &sca3300_set_spimode,
	.op.imu_getid = &sca3300_getid,
	.op.imu_getdata = &sca3300_getdata,
	.acc_range = range_15gl,
	.acc_range_scl = range_inc2,
	.angles = false,
	.locked = true,
	.warn = false,
	.down = false,
};
#endif

imu_host_t *host_ptr;
sFFTData_t fft0 = {
	.id = CAN_FFT_LO,
};

/*
 * Logging data structure
 */
sSensorData_t accel = {
	.id = CAN_IMU_DATA,
};

volatile SPid
xpid = {
	.iMax = 600.0,
	.iMin = -600.0,
	.pGain = 4.0,
	.iGain = 1.05,
},
ypid = {
	.iMax = 600.0,
	.iMin = -600.0,
	.pGain = 4.0,
	.iGain = 1.05,
}, zpid = {
	.iMax = 600.0,
	.iMin = -600.0,
	.pGain = 4.0,
	.iGain = 1.05,
};

volatile SPid xpid, ypid, zpid;
volatile uint16_t tickCount[TMR_COUNT];
static char buffer[FBUFFER_SIZE];
static const char *build_date = __DATE__, *build_time = __TIME__;
const uint32_t update_delay = 5;
uint32_t board_serial_id = 0x35A, cpu_serial_id = 0x1957;

static void fh_start_AT_nodma(void *);
static void sw2_cb(GPIO_PIN, uintptr_t);
static void sw5_cb(GPIO_PIN, uintptr_t);

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void)
{
#ifdef __32MK0512MCJ048__
#ifdef SHOW_LCD
	uint8_t rxe, txe, times = 0;
#endif
	uint8_t alter = 0;
#endif
	bool wait = true, fft_settle = false;
	uint8_t ffti = 0, w = 0;
	uint16_t fft_count = 0;

	/* Initialize all modules */
	SYS_Initialize(NULL);

#ifdef XPRJ_mcj
	//	setup the reset and command pins for the Ethernet adapter
#endif

#ifdef HOST_BOARD
	host_sm();
#endif

	/*
	 * start core-timer for delay_us
	 * ms tick-timer
	 * set cpu serial ID numbers
	 */
	start_tick();

	/*
	 * print the driver version
	 */
	imu0.op.info_ptr(); // print driver version on the serial port
	imu0.op.imu_set_spimode(&imu0); // setup the IMU chip for SPI comms, X updates per second @ selected G range

	/*
	 * start the graphic LCD driver
	 */
	lcd_version();
	init_lcd_drv(D_INIT);
	OledClearBuffer();
	imu0.op.info_ptr();
	eaDogM_WriteStringAtPos(10, 0, imu_buffer);
	fft_version();
	do_fft_version();
	board_serial_id = cpu_serial_id; // this ID could be changed to the ID of the IMU for IMU data transfers
	imu0.board_serial_id = board_serial_id;
	snprintf(buffer, max_buf, "%s Controller %s %X", IMU_ALIAS, IMU_DRIVER, cpu_serial_id);
	eaDogM_WriteStringAtPos(15, 0, buffer);
	snprintf(buffer, max_buf, "Configuration %s", "Sensor node");
	eaDogM_WriteStringAtPos(14, 0, buffer);
	snprintf(buffer, max_buf, "%s Driver %s %s %s", LCD_ALIAS, LCD_DRIVER, build_date, build_time);
	eaDogM_WriteStringAtPos(1, 0, buffer);
	snprintf(buffer, max_buf, "%s Driver %s", CMD_ALIAS, CMD_DRIVER);
	eaDogM_WriteStringAtPos(3, 0, buffer);
	snprintf(buffer, max_buf, "%s Driver %s", REMOTE_ALIAS, REMOTE_DRIVER);
	eaDogM_WriteStringAtPos(4, 0, buffer);
	OledUpdate();
	buzzer_init();
	TMR3_Start();

	/*
	 * check to see if we actually have a working IMU
	 */
	StartTimer(TMR_IMU, IMU_ID_DELAY);
	ADCHS_ChannelConversionStart(ADCHS_CH0);
	ADCHS_ChannelConversionStart(ADCHS_CH1);
	while (!imu0.op.imu_getid(&imu0)) {
		LED_RED_Toggle();
		LED_GREEN_Toggle();
		if (TimerDone(TMR_IMU)) {

			while (wait) {
				if (TimerDone(TMR_IMU)) {
					LED_RED_Toggle();
					LED_GREEN_Toggle();
					snprintf(buffer, max_buf, "IMU NO ID, %d %d ", ADCHS_ChannelResultGet(ADCHS_CH0), ADCHS_ChannelResultGet(ADCHS_CH1));
					eaDogM_WriteStringAtPos(13, 0, buffer);
					eaDogM_WriteStringAtPos(9, 0, imu_buffer);
					OledUpdate();
					StartTimer(TMR_IMU, 200);
					ADCHS_ChannelConversionStart(ADCHS_CH0);
					ADCHS_ChannelConversionStart(ADCHS_CH1);
					if (imu0.op.imu_getid(&imu0)) {
						wait = false;
						break;
					};
				}
			}
		}
	};

	LED_RED_Off();
	LED_GREEN_Off();
	buzzer_trigger(1);
	QEI2_Start();
	POS2CNT = 0;
	GPIO_PinInterruptCallbackRegister(SW2_PIN, sw2_cb, 0);
	GPIO_PinIntEnable(SW2_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
	SW2_InterruptEnable();

	GPIO_PinInterruptCallbackRegister(SW3_PIN, sw2_cb, 0);
	GPIO_PinIntEnable(SW3_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
	SW3_InterruptEnable();

	GPIO_PinInterruptCallbackRegister(SW4_PIN, sw2_cb, 0);
	GPIO_PinIntEnable(SW4_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
	SW4_InterruptEnable();

	GPIO_PinInterruptCallbackRegister(SW5_PIN, sw5_cb, 0);
	GPIO_PinIntEnable(SW5_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
	SW5_InterruptEnable();
	WaitMs(2500);

	TP1_Set(); // ETH modules display trigger

	/* set can-fd extended ID filters and masks */
	canfd_set_filter(board_serial_id, HOST_MAGIC_ID);

#ifdef DEBUG_FILTER
	snprintf(cmd_buffer, max_buf, "%X %X", CAN1_MessageAcceptanceFilterMaskGet(0), CAN1_MessageAcceptanceFilterGet(0));
	snprintf(response_buffer, max_buf, "%X %X", CAN1_MessageAcceptanceFilterMaskGet(1), CAN1_MessageAcceptanceFilterGet(1));
	eaDogM_WriteStringAtPos(6, 0, cmd_buffer);
	eaDogM_WriteStringAtPos(7, 0, response_buffer);
	OledUpdate();
	WaitMs(5000);
#endif

	// loop collecting data
	StartTimer(TMR_LOG, imu0.log_timeout);
	while (true) {
		/* Maintain state machines of all polled MPLAB Harmony modules. */
		SYS_Tasks();

		if (TP1_check()) {
			LED_RED_On();
			OledClearBuffer();
			fh_start_AT_nodma(0);
			eaDogM_WriteStringAtPos(6, 0, cmd_buffer);
			eaDogM_WriteStringAtPos(7, 0, response_buffer);
			OledUpdate();
			WaitMs(5000);
		}

		/*
		 * data logging routine
		 * convert the SPI XYZ response to standard floating point acceleration values and rolling integer time-stamps per measurement
		 */
		if (imu0.update || TimerDone(TMR_LOG)) {

#ifdef SHOW_LCD   
			OledClearBuffer();
#endif
			imu0.op.imu_getdata(&imu0); // read data from the chip
			imu0.update = false;
			getAllData(&accel, &imu0); // convert data from the chip

			accel.xerr = UpdatePI(&xpid, (double) accel.xa);
			accel.yerr = UpdatePI(&ypid, (double) accel.ya);
			accel.zerr = UpdatePI(&zpid, (double) accel.za);
#ifdef SHOW_LOG
			printf("%6.3f,%6.3f,%6.3f,%6.2f,%6.2f,%6.2f,%u,%X,%X\r\n", accel.x, accel.y, accel.z, accel.xa, accel.ya, accel.za, accel.sensortime, imu0.rs, imu0.ss);
#endif
#ifdef SHOW_LCD
			snprintf(buffer, max_buf, "%6.3f,%6.3f,%6.3f, %X, %X\r\n", accel.x, accel.y, accel.z, imu0.rs, imu0.ss);
			eaDogM_WriteStringAtPos(0, 0, buffer);
			snprintf(buffer, max_buf, "%6.2f,%6.2f,%6.2f,%5.1f", accel.xa, accel.ya, accel.za, accel.sensortemp);
			eaDogM_WriteStringAtPos(1, 0, buffer);
			snprintf(buffer, max_buf, "PIC32 IMU Controller %s   %s %s", IMU_DRIVER, build_date, build_time);
			eaDogM_WriteStringAtPos(14, 0, buffer);
			snprintf(buffer, max_buf, "IMU %s", imu_string(&imu0));
			eaDogM_WriteStringAtPos(3, 0, buffer);
			snprintf(buffer, max_buf, "DEV %d", imu0.device);
			eaDogM_WriteStringAtPos(4, 0, buffer);
			snprintf(buffer, max_buf, "RAN %d", imu0.acc_range);
			eaDogM_WriteStringAtPos(5, 0, buffer);
			snprintf(buffer, max_buf, "ANG %s , %d", imu0.angles ? "Yes" : "No", POS2CNT);
			eaDogM_WriteStringAtPos(6, 0, buffer);

			/*
			 * load FFT sample 128 element 8-bit buffer from
			 * 256 element signal buffer
			 * as we process each IMU 3-axis sample
			 * if FFT_mIX is set this is not a pure FFT as it mixes bin data
			 * with sample data for a feedback signature
			 * 
			 * it recomputes with every new IMU data update
			 * unless FFT_MIX is set to false
			 */
			inB[ffti] = 128 + (uint8_t) (fft_gain * (do_fft_dc_x(accel.x) + do_fft_dc_y(accel.y) + do_fft_dc_z(accel.z))); // select one axis for display

			ffti++;
			if (FFT_MIX || ffti == 0) {
				TP3_Set(); // FFT processing timing mark
				do_fft(false); // convert to 256 frequency bins in 8-bit sample buffer
				TP3_Clear(); // end of FFT function
				memset(inB + (N_FFT / 2), 0, N_FFT / 2); // clear upper 128 bytes
				memcpy(fft_buffer, inB, N_FFT); // copy to results buffer
			}
			TP3_Set(); // drawing processing mark
			if (fft_settle) {
				snprintf(buffer, max_buf, "FFTs %3d,%3d ", fft_buffer[ffti], ffti);
				eaDogM_WriteStringAtPos(7, 4, buffer);
			}
			w = 0;
			while (w < 128) {
				fft_draw(w, fft_buffer[w]); // create screen graph from bin data
				w++;
			}
			TP3_Clear(); // end of drawing function
#ifdef SHOW_VG
			q0 = accel.x;
			q1 = accel.y;
			q2 = accel.z;
			q3 = accel.x;
			vector_graph();
#endif
#ifdef SHOW_LA
			{
				uint16_t i = 1;

				LA_gfx(false, false, 0);
				while ((i++ < 1400)) {
					// extra processing loop while waiting for clock time to expire
					LA_gfx(false, false, 1400);
				}
			}
#endif 
			OledUpdate();
#endif
			if (TimerDone(TMR_LOG)) {
				LED_GREEN_Toggle();
			}

#ifdef __32MK0512MCJ048__
#ifdef SHOW_LCD
			CAN1_ErrorCountGet(&txe, &rxe);
			snprintf(buffer, max_buf, "can-fd %X", board_serial_id);
			eaDogM_WriteStringAtPos(11, 0, buffer);
			snprintf(buffer, max_buf, "ErrorT %d", txe);
			eaDogM_WriteStringAtPos(4, 20, buffer);
			snprintf(buffer, max_buf, "ErrorR %d", rxe);
			eaDogM_WriteStringAtPos(5, 20, buffer);
			snprintf(buffer, max_buf, "Can INT %d", CAN1_InterruptGet(1, 0x1f));
			eaDogM_WriteStringAtPos(6, 20, buffer);
			snprintf(buffer, max_buf, "TX Full %s", CAN1_TxFIFOQueueIsFull(1) ? "Y" : "N");
			eaDogM_WriteStringAtPos(7, 20, buffer);
			snprintf(buffer, max_buf, "Update %d", ++times);
			eaDogM_WriteStringAtPos(8, 20, buffer);
			snprintf(buffer, max_buf, "REQ %X", CFD1TXREQ);
			eaDogM_WriteStringAtPos(9, 20, buffer);
			snprintf(buffer, max_buf, "Ce0 %X", CFD1BDIAG0);
			eaDogM_WriteStringAtPos(10, 18, buffer);
			snprintf(buffer, max_buf, "Ce1 %X", CFD1BDIAG1);
			eaDogM_WriteStringAtPos(11, 18, buffer);
			snprintf(buffer, max_buf, "CINT %X, %d, %d, %d", CFD1INT, canfd_num_tx(), canfd_num_stall(), canfd_num_rx());
			eaDogM_WriteStringAtPos(13, 0, buffer);
			//			snprintf(buffer, max_buf,"ER %6.2f, %6.2f, %6.2f", accel.xerr, accel.yerr, accel.zerr);
			//			eaDogM_WriteStringAtPos(12, 0, buffer);
#endif
			canfd_state(CAN_RECEIVE, accel.buffer);
			host_ptr = (imu_host_t *) accel.buffer;
			if (rx_msg_ready) {
				rx_msg_ready = false;
				remote_cmd_decode(host_ptr);
				buzzer_trigger(1);
			}
			eaDogM_WriteStringAtPos(12, 0, hbuffer);

			switch (alter) {
			case 0:
				canfd_state(CAN_TRANSMIT_FD, &imu0);
				alter++;
				break;
			case 1:
				canfd_state(CAN_TRANSMIT_FD, &accel);
				alter++;
				break;
			case 2:
				fft0.id = CAN_FFT_LO;
				if (fft_settle) {
					memcpy(fft0.buffer, &fft_buffer[0], 60);
					canfd_state(CAN_TRANSMIT_FD, &fft0);
				}
				alter++;
				break;
			case 3:
				fft0.id = CAN_FFT_HI;
				if (fft_settle) {
					memcpy(fft0.buffer, &fft_buffer[60], 60);
					canfd_state(CAN_TRANSMIT_FD, &fft0);
				}
				if (!fft_settle && (fft_count++ >= FFT_COUNT)) {
					fft_settle = true;
				}
				alter = 0; // restart data sequence
				break;
			default:
				alter = 0;
				break;
			}
#endif
			StartTimer(TMR_LOG, imu0.log_timeout);
		}
	}

	/* Execution should not come here during normal operation */

	return( EXIT_FAILURE);
}

/*
 * capture and display ETH module network IP address
 * none-DMA serial driver
 */
static void fh_start_AT_nodma(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Start AT commands            ");

#ifdef USR_TCP
	ETH_CFG_Clear();
	WaitMs(30);
	ETH_CFG_Set();
	WaitMs(1500); // wait until the module is back online
#else
	//	ETH_CFG_Clear();
	WaitMs(500);
	//	ETH_CFG_Set();
	WaitMs(4500); // wait until the module is back online
#endif

	// AT command mode
	UART1_Write("+++", 3); // send data to the ETH module
	WaitMs(20);
	UART1_Write("a", 1); // send data to the ETH module
	WaitMs(20);
	if (UART1_ReceiverIsReady()) { // check to see if we have a response
		// send a Ethernet connection query
		UART1_Write("AT+WANN\r", 8); // send data to the ETH module
		// put the result in a buffer for the GLCD to display
		UART1_Read(response_buffer, 30);
	} else { // nothing
		snprintf(response_buffer, max_buf, "AT command failed           ");
		//		ETH_RESET_Clear();
		WaitMs(200);
		//		ETH_RESET_Set();
	}
	/*
	 * AT mode will timeout after 30 seconds and go back to transparent data mode
	 */
	WaitMs(500);
	UART1_ErrorGet(); // clear UART junk
}

void sw2_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + 70000)) {
		buzzer_trigger(1);
		dbounce = TMR3_CounterGet();
		POS2CNT--;
	} else {
		//		dbounce = TMR3_CounterGet();
	}

}

void sw5_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + 70000)) {
		buzzer_trigger(1);
		dbounce = TMR3_CounterGet();
		POS2CNT++;
	} else {
		//		dbounce = TMR3_CounterGet();
	}

}
/*******************************************************************************
 End of File
 */
