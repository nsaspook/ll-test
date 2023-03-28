#include "imupic32mcj.h"
#include "host.h"

/*
 * Sensor network host
 * it takes the four sensor data packets and converts that to a database format
 * for a network client reading the data-stream. The physical CAN-FD hw/sw
 * system can handle 50 sensors but the 460800 serial to ETH module connection
 * bottlenecks. 
 * The Max sensor count is ~6 because of the serial ttl speed bottleneck
 */

#define USE_SERIAL_DMA

#ifdef USE_SERIAL_DMA
#ifdef XPRJ_bma400
#include "config/bma400/peripheral/dmac/plib_dmac.h"
#endif

#ifdef XPRJ_mcj
#include "config/mcj/peripheral/dmac/plib_dmac.h"
#endif

#ifdef XPRJ_mcj_remote
#include "config/mcj_remote/peripheral/dmac/plib_dmac.h"
#endif

#endif

/* Application's state machine enum */
typedef enum {
	APP_STATE_CAN_RECEIVE,
	APP_STATE_CAN_TRANSMIT,
	APP_STATE_CAN_IDLE,
	APP_STATE_CAN_XFER_SUCCESSFUL,
	APP_STATE_CAN_XFER_ERROR
} APP_STATES;

typedef enum {
	CAN_NULL,
	CAN_IMU_DATA,
	CAN_IMU_INFO,
	CAN_IMU_RAW,
	CAN_IMU_GET,
	CAN_IMU_SET,
	CAN_STAT,
	CAN_MISC,
	CAN_FFT_LO,
	CAN_FFT_HI,
} CANFD_MESSAGE;

static sSensorData_t *accel;
static imu_cmd_t *imu;
static sFFTData_t *fft;
static char buffer[RBUFFER_SIZE];
char cmd_buffer[RBUFFER_SIZE] = "Waiting for commands";
char response_buffer[RBUFFER_SIZE] = " ";

imu_host_t host0 = {
	.id = CAN_MISC,
	.cmd = CMD_IDLE,
	.secret = HOST_SECRET,
};

uint32_t fft_bin_total(sFFTData_t *, uint32_t);
double approxRollingAverage(double avg, double new_sample);

/* Variable to save application state */
static volatile APP_STATES state = APP_STATE_CAN_IDLE;
/* Variable to save Tx/Rx transfer status and context */
static volatile uint32_t status = 0;
static volatile uint32_t xferContext = 0;
/* Variable to save Tx/Rx message */
static uint32_t messageID = 0;
static uint8_t message[64];
static uint8_t messageLength = 0;
static uint8_t rx_message[64];
static uint32_t rx_messageID = 0;
static uint8_t rx_messageLength = 0;
static uint32_t timestamp = 0;
static volatile uint32_t tx_num = 0;
static CANFD_MSG_RX_ATTRIBUTE msgAttr = CANFD_MSG_RX_DATA_FRAME;

const char *build_date = __DATE__, *build_time = __TIME__;
uint64_t host_cpu_serial_id = 0x1957;
uint32_t times = 0;

uint8_t spid[] = {0xb5, 0x62, 0x0A, 0x04, 0x00};

static volatile bool uart1_dma_busy = false, uart2_dma_busy = false, rec_message = false,
	send_wait = false, recv_error = false;
char uart_buffer[256];
extern t_cli_ctx cli_ctx; // command buffer

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

/* This function will be called by CAN PLIB when transfer is completed */
// *****************************************************************************

/* void APP_CAN_Callback_h(uintptr_t context)

  Summary:
    Function called by CAN PLIB upon transfer completion

  Description:
    This function will be called by CAN PLIB when transfer is completed.
    In this function, current state of the application is obtained by context
    parameter. Based on current state of the application and CAN error
    state, next state of the application is decided.

  Remarks:
    None.
 */
void APP_CAN_Callback_h(uintptr_t);
void APP_CAN_Callback_hs(uintptr_t);
void APP_CAN_Error_Callback_h(uintptr_t);
void APP_CAN_Error_Callback_hs(uintptr_t);

void fh_start_AT(void *a_data);

void APP_CAN_Callback_h(uintptr_t context)
{
	xferContext = context;
	uint8_t rxe, txe;

	/* Check CAN Status */
	status = CAN1_ErrorGet();
	CAN1_ErrorCountGet(&txe, &rxe);
#ifdef DEBUG_can_callback1
	char buffer[256] = " ";
	snprintf(buffer, max_buf, "CB %d,TE %d,RE %d,ER %X,DI %X", status, txe, rxe, CFD1TREC, CFD1BDIAG1);
	eaDogM_WriteStringAtPos(5, 0, buffer);
#endif

	if ((status & (CANFD_ERROR_TX_RX_WARNING_STATE | CANFD_ERROR_RX_WARNING_STATE |
		CANFD_ERROR_TX_WARNING_STATE | CANFD_ERROR_RX_BUS_PASSIVE_STATE |
		CANFD_ERROR_TX_BUS_PASSIVE_STATE | CANFD_ERROR_TX_BUS_OFF_STATE)) == CANFD_ERROR_NONE) {
		switch ((APP_STATES) context) {
		case APP_STATE_CAN_RECEIVE:
		{
			rec_message = true;
			break;
		}
		case APP_STATE_CAN_TRANSMIT:
		{
			TP2_Clear();
			LED_RED_Set();
			tx_num++;
			break;
		}
		default:
			break;
		}
	} else {
		state = APP_STATE_CAN_XFER_ERROR;
	}
}

void APP_CAN_Callback_hs(uintptr_t context)
{
	xferContext = context;

	TP2_Clear();
	tx_num++;
	send_wait = false;
}

void APP_CAN_Error_Callback_h(uintptr_t context)
{
	xferContext = context;

	/* Check CAN Status */
	status = CAN1_ErrorGet();
#ifdef DEBUG_can_callback
	char buffer[256] = " ";
	snprintf(buffer, max_buf, "CEB status %3X Err ", status);
	eaDogM_WriteStringAtPos(5, 0, buffer);
#endif
	state = APP_STATE_CAN_XFER_ERROR;
	recv_error = true;
}

void APP_CAN_Error_Callback_hs(uintptr_t context)
{
	xferContext = context;

	LED_RED_Set();
}

#ifdef USE_SERIAL_DMA
void UART1DmaChannelHandler_State(DMAC_TRANSFER_EVENT, uintptr_t);
void UART1DmaWrite(const char *, uint32_t);

/*
 * end of uart buffer complete flag handler callback
 * interrupt handler for the completion of buffer transfer.
 */
void UART1DmaChannelHandler_State(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
{
	uart1_dma_busy = false;
}

/*
 * DMA uart serial function
 * triggers the DMA transfer and returns, only one interrupt happens at the end of transfer
 */
void UART1DmaWrite(const char * buffer, uint32_t len)
{
	while (uart1_dma_busy || U1STAbits.UTXBF) { // should never wait in normal operation
	};

	uart1_dma_busy = true; // in process flag
	DMAC_ChannelTransfer(DMAC_CHANNEL_7, (const void *) buffer, (size_t) len, (const void*) &U1TXREG, (size_t) 1, (size_t) 1);
}
#endif

/*
 * CAN-FD vibration sensor host to network state machine
 */

int host_sm(void)
{
	uint8_t count = 0;
	bool msg_ready = false;
	uint64_t * hcid = (uint64_t *) & DEVSN2; // set pointer to 64-bit cpu serial number
	uint32_t wait_count = 0, recv_count = 0, msg_error = 0;

	//ETH_RESET_Clear();

	/* 
	 * Start system tick timer 
	 * ms tick-timer
	 * set cpu serial ID numbers
	 */
	start_tick();

	host_cpu_serial_id = *hcid; // get the CPU device 64-bit serial number and use that as a HOST ID

#ifdef USE_SERIAL_DMA
	DMAC_ChannelCallbackRegister(DMAC_CHANNEL_7, UART1DmaChannelHandler_State, 0); // end of UART buffer transfer interrupt function usart1
#endif

	/* Prepare the default message to send */
	for (count = 0; count < 64; count++) {
		message[count] = count;
	}
	//ETH_RESET_Set();

	/*
	 * start the graphic LCD driver
	 */
	init_lcd_drv(D_INIT);
	OledClearBuffer();
	snprintf(buffer, max_buf, "%s Driver %s %s %s", LCD_ALIAS, LCD_DRIVER, build_date, build_time);
	eaDogM_WriteStringAtPos(0, 0, buffer);
	snprintf(buffer, max_buf, "%s Controller %s %llX", HOST_ALIAS, HOST_DRIVER, host_cpu_serial_id);
	eaDogM_WriteStringAtPos(2, 0, buffer);
	snprintf(buffer, max_buf, "%s Driver %s", CMD_ALIAS, CMD_DRIVER);
	eaDogM_WriteStringAtPos(3, 0, buffer);
	snprintf(buffer, max_buf, "%s Driver %s", REMOTE_ALIAS, REMOTE_DRIVER);
	eaDogM_WriteStringAtPos(4, 0, buffer);
	snprintf(buffer, max_buf, "Configuration %s", "Host node");
	eaDogM_WriteStringAtPos(14, 0, buffer);
	OledUpdate();

	LED_RED_Off();
	TP1_Set();
	WaitMs(500);

	UART1_ErrorGet(); // clear UART junk
	scmd_init(); // start command parser

	StartTimer(TMR_HOST, host_lcd_update);
	StartTimer(TMR_REPLY, host_xmit_wait);

	while (true) {

		/* Maintain state machines of all polled MPLAB Harmony modules. */
		SYS_Tasks();

		LED_GREEN_Toggle();
		/*
		 * Short TP1 to ground for at least 5 seconds to trigger a IP query to the ETH module
		 * release short after red LED stays on
		 */
		if (TP1_check()) {
			LED_RED_On();
			OledClearBuffer();
			fh_start_AT(&cli_ctx);
		}

		/*
		 * read serial port 1 for command data
		 */
		cli_read(&cli_ctx);

		snprintf(buffer, max_buf, "Processing CAN-FD %4i  %4i", wait_count++, state);
		eaDogM_WriteStringAtPos(12, 0, buffer);

		msg_ready = CAN1_InterruptGet(2, CANFD_FIFO_INTERRUPT_TFNRFNIF_MASK);
		if (msg_ready) {

			// Waiting for message
#ifdef HOST_BOARD
			CAN1_CallbackRegister(APP_CAN_Callback_h, (uintptr_t) APP_STATE_CAN_RECEIVE, 2);
			CAN1_ErrorCallbackRegister(APP_CAN_Error_Callback_h, (uintptr_t) APP_STATE_CAN_RECEIVE);
#endif
			memset(rx_message, 0x00, sizeof(rx_message));

			/* Receive New Message */
			if (CAN1_MessageReceive(&rx_messageID, &rx_messageLength, rx_message, &timestamp, 2, &msgAttr) == false) {
				snprintf(buffer, max_buf, "Receive request has failed");
				eaDogM_WriteStringAtPos(8, 0, buffer);
			}
			/* Application can do other task here */
			snprintf(buffer, max_buf, "CAN-FD  received %i", recv_count++);
			eaDogM_WriteStringAtPos(13, 0, buffer);
			wait_count = 0;
			state = APP_STATE_CAN_XFER_SUCCESSFUL;
		} else {
			{
				/* Application can do other task here */
				static double avg_result = 1.0;

				avg_result = approxRollingAverage(avg_result, (double) wait_count++);
				if (avg_result > 999999.0) { // limit display value
					avg_result = 999999.0;
				}
				snprintf(buffer, max_buf, "Waiting for data packet %6i", (int32_t) avg_result);
				eaDogM_WriteStringAtPos(14, 0, buffer);
			}
		}

		/* Check the application's current state. */
		switch (state) {
		case APP_STATE_CAN_IDLE:
		{
			// do nothing, loop again
			break;
		}
		case APP_STATE_CAN_XFER_SUCCESSFUL:
		{
			wait_count = 0;
			snprintf(buffer, max_buf, "                             ");
			eaDogM_WriteStringAtPos(14, 0, buffer);

			/*
			 * signal uart contention
			 */
			uint32_t contention = 0;
			LED_RED_Off();
			while (uart1_dma_busy || U1STAbits.UTXBF) { // uart flow-control RED led
				if (contention++ == uart_wait) {
					LED_RED_Toggle();
				}
			};

			if ((APP_STATES) xferContext == APP_STATE_CAN_RECEIVE) {
				/* Print message to LCD Console buffer and CAN-FD uart_buffer */
				uint8_t length = rx_messageLength;
				uint16_t * mtype = (uint16_t *) & rx_message[0];
				snprintf(uart_buffer, max_buf, "-1, Bad  Message ID code\r\n");
				if (*mtype == CAN_IMU_DATA) {
					accel = (sSensorData_t *) rx_message;
					length++;
					snprintf(uart_buffer, max_buf, "%3d,%7X,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.1f,%s\r\n",
						accel->id, rx_messageID, accel->x, accel->y, accel->z, accel->xa, accel->ya, accel->za, accel->xerr, accel->yerr, accel->zerr, (double) accel->sensortime, IMU_ALIAS);
				}
				if (*mtype == CAN_IMU_INFO) {
					imu = (imu_cmd_t *) rx_message;
					imu->host_serial_id = host_cpu_serial_id;
					snprintf(uart_buffer, max_buf, "%3d,%7X,%7X,%3d,%3d,%3d,%18llX,%s\r\n",
						imu->id, imu->board_serial_id, rx_messageID, imu->device, imu->acc_range, imu->features, host_cpu_serial_id, IMU_ALIAS);
					snprintf(buffer, max_buf, "ID:%3d,%7X,%7X", imu->id, imu->board_serial_id, rx_messageID);
					eaDogM_WriteStringAtPos(15, 0, buffer);
				}
				if (*mtype == CAN_FFT_LO) {
					fft = (sFFTData_t *) rx_message;
					snprintf(uart_buffer, max_buf, "%3d,%7X,%3d,%s\r\n", fft->id, rx_messageID, fft_bin_total(fft, 16), IMU_ALIAS);
				}
				if (*mtype == CAN_FFT_HI) {
					fft = (sFFTData_t *) rx_message;
					snprintf(uart_buffer, max_buf, "%3d,%7X,%3d,%s\r\n", fft->id, rx_messageID, fft_bin_total(fft, 0), IMU_ALIAS);
				}
				if (*mtype == CAN_NULL) {
					snprintf(uart_buffer, max_buf, "0,NULL Message with 0 ID code\r\n");
				}
				UART1DmaWrite(uart_buffer, strlen(uart_buffer)); // send data to the ETH module
			} else if ((APP_STATES) xferContext == APP_STATE_CAN_TRANSMIT) {
			}
			state = APP_STATE_CAN_IDLE;
			break;
		}
		case APP_STATE_CAN_XFER_ERROR:
		{
			if ((APP_STATES) xferContext == APP_STATE_CAN_RECEIVE) {
				snprintf(buffer, max_buf, "Err,recv message %i, %d", ++msg_error, xferContext);
			} else {
				snprintf(buffer, max_buf, "Failed  %d %d         ", ++msg_error, xferContext);
			}
			eaDogM_WriteStringAtPos(9, 0, buffer);
			state = APP_STATE_CAN_IDLE;
			break;
		}
		default:
			break;
		}

		if (recv_error) {
			recv_error = false;
			clear_can_errors();
		}

		/*
		 * not currently used
		 * host to sensor messages happen at screen updates
		 */
		if (TimerDone(TMR_REPLY)) {
			StartTimer(TMR_REPLY, host_xmit_wait);
		}


		if (TimerDone(TMR_HOST)) {
			StartTimer(TMR_HOST, host_lcd_update);
			if (rec_message) {
				rec_message = false;
				if (CAN1_InterruptGet(1, CANFD_FIFO_INTERRUPT_TFNRFNIF_MASK)) {
					send_wait = true;
					send_from_host(HOST_MAGIC_ID); // Master broadcast ID
					while (send_wait) {
					};
				}
			}
			eaDogM_WriteStringAtPos(6, 0, cmd_buffer);
			eaDogM_WriteStringAtPos(7, 0, response_buffer);
			snprintf(buffer, max_buf, "Tx CAN-FD %8X   %6i", messageID, tx_num);
			eaDogM_WriteStringAtPos(11, 0, buffer);
			OledUpdate();
		}
	}

	/* Execution should not come here during normal operation */

	return( EXIT_FAILURE);
}

uint32_t fft_bin_total(sFFTData_t * fftt, uint32_t trim)
{
	uint32_t total = 0;

	if (trim > 25) {
		trim = 25;
	}

	for (int32_t i = trim; i < 50; i++) {
		total += fftt->buffer[i];
	}
	return total;
}

double approxRollingAverage(double avg, double new_sample)
{

	avg -= avg / avg_samples;
	avg += new_sample / avg_samples;

	return avg;
}

/*
 * capture and display ETH module network IP address
 * DMA serial driver
 */
void fh_start_AT(void *a_data)
{
	snprintf(cmd_buffer, max_buf, "Start AT commands            ");
//	UART_SERIAL_SETUP setup = {
//		.baudRate = 115200,
//		.parity = UART_PARITY_NONE,
//		.dataWidth = UART_DATA_8_BIT,
//		.stopBits = UART_STOP_1_BIT,
//	};

	// wait for send uart buffer to finish
	uint32_t contention = 0;
	while (uart1_dma_busy || U1STAbits.UTXBF) { // uart flow-control RED led
		if (contention++ == uart_wait) {
		}
	};
//	UART1_SerialSetup(&setup, 60000000);

	// put the ETH module in config mode
	U1MODECLR = _U1MODE_ON_MASK; // turn off UART

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

	U1MODESET = _U1MODE_ON_MASK; // re-enable UART

	// AT command mode
	UART1DmaWrite("+++", 3); // send data to the ETH module
	WaitMs(20);
	UART1DmaWrite("a", 1); // send data to the ETH module
	WaitMs(20);
	if (UART1_ReceiverIsReady()) { // check to see if we have a response
		// send a Ethernet connection query
		UART1DmaWrite("AT+WANN\r", 8); // send data to the ETH module
		UART1_Read(&response_buffer, 30); // read serial response from module
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

bool TP1_check(void)
{
	static uint32_t debounce = 0;

	if (TP1_Get() == 0) {
		debounce++;
		if (debounce >= debounce_delay) {
			debounce = debounce_delay + 1;
			return true;
		} else {
			return false;
		}
	} else {
		debounce = 0;
		return false;
	}
}

void send_from_host(uint32_t hostid)
{
	bool msg_ready;

	msg_ready = CAN1_InterruptGet(1, 0x1f);

	if (msg_ready) {
		/*
		 * use CAN-FD compatible 29-bit serial ID numbers
		 */
#ifdef INT_BOARD
		CAN1_CallbackRegister(APP_CAN_Callback_hs, (uintptr_t) APP_STATE_CAN_TRANSMIT, 1);
		CAN1_ErrorCallbackRegister(APP_CAN_Error_Callback_hs, (uintptr_t) APP_STATE_CAN_TRANSMIT);
#endif
		messageID = hostid; // serial of the MPU
		messageLength = 64;
		host0.host_serial_id = DEVSN0 & 0x1fffffff;

		TP2_Set();
		if (CAN1_MessageTransmit(messageID, messageLength, (void *) &host0, 1, CANFD_MODE_FD_WITH_BRS, CANFD_MSG_TX_DATA_FRAME) == false) {
		}
		LED_RED_Clear();
		LED_GREEN_Toggle();
	} else {
	}
}