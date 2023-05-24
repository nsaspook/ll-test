
#include "imupic32mcj.h"

static uint32_t delay_freq = 0;
volatile uint32_t bno08x_int_count = 0;

#ifdef __32MK0512MCJ048__
//void qei_index_cb(QEI_STATUS, uintptr_t);

/*
 * on QEI index trigger function
 */
//void qei_index_cb(QEI_STATUS status, uintptr_t context)
//{

//}
#endif

/*
 * configure the SPI port bit size for data transfers
 */
uint8_t set_imu_bits(void)
{
	uint8_t imu_bits = 8;

#ifdef BNO086 // mode 3
#define SPI2_CON_MSTEN                      (1 << _SPI2CON_MSTEN_POSITION)
#define SPI2_CON_CKP                        (1 << _SPI2CON_CKP_POSITION)
#define SPI2_CON_CKE                        (0 << _SPI2CON_CKE_POSITION)
#define SPI2_CON_MODE_32_MODE_16            (0 << _SPI2CON_MODE16_POSITION)
#define SPI2_CON_ENHBUF                     (1 << _SPI2CON_ENHBUF_POSITION)
#define SPI2_CON_MCLKSEL                    (1 << _SPI2CON_MCLKSEL_POSITION)
#define SPI2_CON_MSSEN                      (0 << _SPI2CON_MSSEN_POSITION)
#define SPI2_CON_SMP                        (0 << _SPI2CON_SMP_POSITION)

	/* BAUD Rate register Setup */
	SPI2BRG = 19; // 3MHz
#else // mode 0
#define SPI2_CON_MSTEN                      (1 << _SPI2CON_MSTEN_POSITION)
#define SPI2_CON_CKP                        (0 << _SPI2CON_CKP_POSITION)
#define SPI2_CON_CKE                        (1 << _SPI2CON_CKE_POSITION)
#define SPI2_CON_ENHBUF                     (1 << _SPI2CON_ENHBUF_POSITION)
#define SPI2_CON_MCLKSEL                    (1 << _SPI2CON_MCLKSEL_POSITION)
#define SPI2_CON_MSSEN                      (0 << _SPI2CON_MSSEN_POSITION)
#define SPI2_CON_SMP                        (0 << _SPI2CON_SMP_POSITION)

	SPI2BRG = 7; // 8MHz

#ifdef SPI2_32BIT
	imu_bits = 32;
#define SPI2_CON_MODE_32_MODE_16            (3 << _SPI2CON_MODE16_POSITION)
#else
#define SPI2_CON_MODE_32_MODE_16            (0 << _SPI2CON_MODE16_POSITION)
#endif
#endif

	SPI2CONSET = (SPI2_CON_MSSEN | SPI2_CON_MCLKSEL | SPI2_CON_ENHBUF | SPI2_CON_MODE_32_MODE_16 | SPI2_CON_CKE | SPI2_CON_CKP | SPI2_CON_MSTEN | SPI2_CON_SMP);

	return imu_bits;
}

/*
 * setup external interrupt #2 for IMU BMA4x0 data update interrupt trigger output
 */
void init_imu_int(const imu_cmd_t * imu)
{
	if (imu) {
		INTCONCLR = _INTCON_INT2EP_MASK; //External interrupt on falling edge
		IFS0CLR = _IFS0_INT2IF_MASK; // Clear the external interrupt flag
		EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_2, update_imu_int1, (uintptr_t) imu);
		EVIC_ExternalInterruptEnable(EXTERNAL_INT_2);
	}
}

/*
 * user callback function per BMA4x0 data interrupt
 * update pacing flag from IMU ISR
 */
void update_imu_int1(uint32_t a, uintptr_t context)
{
	imu_cmd_t * imu = (imu_cmd_t *) context;
	static int8_t i = 0;
	static uint8_t tog = 0;

	if (imu) {
		if (!i++) {

		}
		if (++tog >= 0) {
			imu->update = true;
			tog = 0;
			LED_GREEN_Toggle();
		}
	}
}

/*
 * setup external interrupt #2 for IMU BNO08X data update interrupt trigger output
 */
void init_imu_int_bno(const imu_cmd_t * imu)
{
	if (imu) {
		INTCONCLR = _INTCON_INT2EP_MASK; //External interrupt on falling edge
		IFS0CLR = _IFS0_INT2IF_MASK; // Clear the external interrupt flag
		EVIC_ExternalInterruptCallbackRegister(EXTERNAL_INT_2, update_imu_int_bno, (uintptr_t) imu);
		EVIC_ExternalInterruptEnable(EXTERNAL_INT_2);
	}
}

/*
 * user callback function per BNO08X data interrupt
 * update pacing flag from IMU ISR
 * passes an pointer to the IMU data object
 */
void update_imu_int_bno(uint32_t a, uintptr_t context)
{
	imu_cmd_t * imu = (imu_cmd_t *) context;

	if (imu) {
		imu->update = true;
		bno08x_int_count++;
		LED_GREEN_Set();
	}
}

/*
 * microsecond busy wait delay, 90 seconds MAX
 * Careful, uses core timer
 */
void delay_us(uint32_t us)
{
	// Convert microseconds us into how many clock ticks it will take
	us *= delay_freq; // Core Timer updates every 2 ticks
	_CP0_SET_COUNT(0); // Set Core Timer count to 0
	while (us > _CP0_GET_COUNT()) {
	}; // Wait until Core Timer count reaches the number we calculated earlier
}

/*
 * start core-timer for delay_us
 */
void start_tick(void)
{
	/* Start system tick timer */
	CORETIMER_Start();
	delay_freq = CORETIMER_FrequencyGet() / 1000000;

	/*
	 * software timers interrupt setup
	 * using tickCount
	 */
	TMR6_CallbackRegister(timer_ms_tick, 0);
	TMR6_Start(); // software timers counter

#ifdef __32MK0512MCJ048__
	TMR9_Start(); // IMU time-stamp counter
	//	QEI2_CallbackRegister(qei_index_cb, 0);
	//	QEI2_Start();
#endif
#ifdef __32MZ1025W104132__
	TMR2_Start(); // IMU time-stamp counter
#endif

#ifdef __32MZ1025W104132__
	cpu_serial_id = USERID & 0x1fffffff; // get CPU device 32-bit serial number and convert that to 29 - bit ID for CAN - FD
#else
	cpu_serial_id = DEVSN1 & 0x1fffffff; // get CPU device 32-bit serial number and convert that to 29 - bit ID for CAN - FD
#endif
}

void canfd_set_filter(uint32_t fil0, uint32_t fil1)
{
	/* Place CAN module in configuration mode */
	CFD1CONbits.REQOP = 4;
	while (CFD1CONbits.OPMOD != 4);
	// disable filters for configuration
	CFD1FLTCON0bits.FLTEN0 = 0;
	CFD1FLTCON0bits.FLTEN1 = 0;
	CFD1TDCbits.TDCMOD = 2;
	CFD1FLTCON0bits.F0BP = 2; // message stored in FIFO2
	CFD1FLTCON0bits.F1BP = 2; // message stored in FIFO2
	// extended identifier address
	CFD1FLTOBJ0bits.EXIDE = 1;
	CFD1FLTOBJ1bits.EXIDE = 1;
	// match mask to address type
	CFD1MASK0bits.MIDE = 1;
	CFD1MASK1bits.MIDE = 1;
	CAN1_MessageAcceptanceFilterMaskSet(0, (~fil0) & 0x1fffffff); // generate mask from ID
	CAN1_MessageAcceptanceFilterMaskSet(1, (~fil1) & 0x1fffffff);
	CAN1_MessageAcceptanceFilterSet(0, fil0);
	CAN1_MessageAcceptanceFilterSet(1, fil1);
	// enable filters after configuration
	CFD1FLTCON0bits.FLTEN0 = 1;
	CFD1FLTCON0bits.FLTEN1 = 1;
	/* Place the CAN module in Normal mode */
	CFD1CONbits.REQOP = 0;
	while (CFD1CONbits.OPMOD != 0);
}

void clear_can_errors(void)
{
	/*
	 * clear application error bits
	 */
	CFD1INTbits.SERRIF = 0;
	CFD1INTbits.CERRIF = 0;
	CFD1INTbits.IVMIF = 0;
	CFD1INTbits.WAKIF = 0;
	CFD1INTbits.MODIF = 0;
	CFD1INTbits.TBCIF = 0;
}

void set_can_retran(void)
{
	CFD1FIFOCON1bits.TXAT = CFD_RETRAN;
	CFD1FIFOCON2bits.TXAT = CFD_RETRAN;
	CFD1FIFOCON3bits.TXAT = CFD_RETRAN;
	CFD1FIFOCON4bits.TXAT = CFD_RETRAN;
	CFD1CONbits.RTXAT = CFD_RETRAN_MODE;
}