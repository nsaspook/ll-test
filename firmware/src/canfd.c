#include "canfd.h"

/* Variable to save application state */
static APP_STATES state = APP_STATE_CAN_USER_INPUT;
/* Variable to save Tx/Rx transfer status and context */
static volatile uint32_t status = 0;
static volatile uint32_t xferContext = 0;
/* Variable to save Tx/Rx message */
static uint32_t messageID = 0, num_tx = 0, num_stall = 0, sensor_rec = 0;
static uint8_t message[64];
static uint8_t messageLength = 0;
static uint8_t rx_message[64];
static uint32_t rx_messageID = 0;
static uint8_t rx_messageLength = 0;
static uint32_t timestamp = 0;
static CANFD_MSG_RX_ATTRIBUTE msgAttr = CANFD_MSG_RX_DATA_FRAME;

volatile bool tx_msg_ready = false;
volatile bool rx_msg_ready = false;

// *****************************************************************************
// *****************************************************************************
// Section: Local functions
// *****************************************************************************
// *****************************************************************************

/* This function will be called by CAN PLIB when transfer is completed */
// *****************************************************************************

/* void APP_CAN_Callback(uintptr_t context)

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
void APP_CAN_Callback(uintptr_t context)
{

	xferContext = context;

	/* Check CAN Status */
	status = CAN1_ErrorGet();

	if ((status & (CANFD_ERROR_TX_RX_WARNING_STATE | CANFD_ERROR_RX_WARNING_STATE |
		CANFD_ERROR_TX_WARNING_STATE | CANFD_ERROR_RX_BUS_PASSIVE_STATE |
		CANFD_ERROR_TX_BUS_PASSIVE_STATE | CANFD_ERROR_TX_BUS_OFF_STATE)) == CANFD_ERROR_NONE) {
		switch ((APP_STATES) context) {
		case APP_STATE_CAN_RECEIVE:
		case APP_STATE_CAN_TRANSMIT:
		{
			LED_RED_Toggle();
			state = APP_STATE_CAN_XFER_SUCCESSFUL;
			break;
		}
		default:
			break;
		}
	} else {
		state = APP_STATE_CAN_XFER_ERROR;
	}
}

void APP_CAN_Error_Callback(uintptr_t context)
{
	xferContext = context;

	/* Check CAN Status */
#ifndef CANDEV2
	status = CAN1_ErrorGet();
#else
	status = CAN2_ErrorGet();
#endif
}



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int canfd_state(CANFD_STATES mode, void * can_buffer)
{
	static uint8_t user_input = 0;
	static uint8_t count = 0;
	uint16_t * mtype = (uint16_t *) can_buffer;
	bool msg_ready = false;

	/* Prepare the message to send */
	for (count = 0; count < 64; count++) {
		message[count] = count;
	}

	while (true) {
		LED_RED_Off();
		LED_GREEN_Off();
		if (state == APP_STATE_CAN_USER_INPUT) {
			user_input = mode;

			switch (user_input) {
			case CAN_TRANSMIT_FD:
				msg_ready = CAN1_InterruptGet(1, CANFD_FIFO_INTERRUPT_TFNRFNIF_MASK);

				if (msg_ready) {
					state = APP_STATE_CAN_IDLE;
					tx_msg_ready = true;

					/*
					 * use CAN-FD compatible 29-bit serial ID numbers
					 */
					if (*mtype == CAN_IMU_DATA || *mtype == CAN_FFT_LO || *mtype == CAN_FFT_HI) {
						messageID = board_serial_id; // serial of the IMU device
					} else {
						messageID = cpu_serial_id; // serial of the MPU
					}
					messageLength = 64;
					num_tx++;
					if (CAN1_MessageTransmit(messageID, messageLength, can_buffer, 1, CANFD_MODE_FD_WITH_BRS, CANFD_MSG_TX_DATA_FRAME) == false) {
					}
					LED_GREEN_Toggle();
				} else {
					state = APP_STATE_CAN_IDLE;
					num_stall++;
				}
				break;
			case CAN_TRANSMIT_N:
				state = APP_STATE_CAN_IDLE;
				messageID = 0x369;
				messageLength = 8;
				if (CAN1_MessageTransmit(messageID, messageLength, can_buffer, 1, CANFD_MODE_NORMAL, CANFD_MSG_TX_DATA_FRAME) == false) {
				}
				break;
			case CAN_RECEIVE:
				msg_ready = CAN1_InterruptGet(2, CANFD_FIFO_INTERRUPT_TFNRFNIF_MASK);
				if (msg_ready) {
					state = APP_STATE_CAN_IDLE;
					rx_msg_ready = true;
					memset(rx_message, 0x00, sizeof(rx_message));
					/* Receive New Message */
					if (CAN1_MessageReceive(&rx_messageID, &rx_messageLength, can_buffer, &timestamp, 2, &msgAttr) == false) {
					} else {
						sensor_rec++;
					}
				} else {
					state = APP_STATE_CAN_IDLE;
				}
				break;
			case CAN_IDLE:
				break;
			default:
				break;
			}
		}
		/* Check the application's current state. */
		switch (state) {
		case APP_STATE_CAN_IDLE:
		{
			/* Application can do other task here */
			state = APP_STATE_CAN_USER_INPUT;
			return xferContext;
			break;
		}
		case APP_STATE_CAN_XFER_SUCCESSFUL:
		{
			if ((APP_STATES) xferContext == APP_STATE_CAN_RECEIVE) {
			} else if ((APP_STATES) xferContext == APP_STATE_CAN_TRANSMIT) {
			}
			state = APP_STATE_CAN_IDLE;
			break;
		}
		case APP_STATE_CAN_XFER_ERROR:
		{
			if ((APP_STATES) xferContext == APP_STATE_CAN_RECEIVE) {
			} else {
			}
			state = APP_STATE_CAN_IDLE;
			break;
		}
		default:
			break;
		}

	}

	/* Execution should not come here during normal operation */

	return( EXIT_FAILURE);
}

uint32_t canfd_num_tx(void)
{
	return num_tx;
}

uint32_t canfd_num_rx(void)
{
	return sensor_rec;
}

uint32_t canfd_num_stall(void)
{
	return num_stall;
}

/*******************************************************************************
 End of File
 */
