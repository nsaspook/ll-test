/*
 * Human input device handler, GLCD screen back-light blank and FSM
 */
#include "hid.h"

H_type H = {
	.dis_alt = false,
	.dis_unblank = false,
	.show_la = true,
	.la_mod = true,
	.dis_reset = false,
	.silent = false,
	.hid_state = H_init,
};

static void hid_blank_cb(uint32_t, uintptr_t);
static	void sw2_cb(GPIO_PIN, uintptr_t);
static	void sw4_cb(GPIO_PIN, uintptr_t);
static	void sw5_cb(GPIO_PIN, uintptr_t);

/*
 * setup and start the needed hardware and software
 */
void hid_init(H_STATE hs)
{
	H.hid_state = hs;
	switch (hs) {
	case H_init:
		TMR3_Start(); // debounce counter
		TMR4_CallbackRegister(hid_blank_cb, 0);
		TMR4_Start(); // screen blank counter
		GPIO_PinInterruptCallbackRegister(SW2_PIN, sw2_cb, 0);
		GPIO_PinIntEnable(SW2_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
		SW2_InterruptEnable();

		GPIO_PinInterruptCallbackRegister(SW4_PIN, sw4_cb, 0);
		GPIO_PinIntEnable(SW4_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
		SW4_InterruptEnable();

		GPIO_PinInterruptCallbackRegister(SW5_PIN, sw5_cb, 0);
		GPIO_PinIntEnable(SW5_PIN, GPIO_INTERRUPT_ON_FALLING_EDGE);
		SW5_InterruptEnable();
		break;
	case H_zero_blank:
		BLANK_COUNTER = 0x0; /* Clear counter to restart blanking period */
		break;
	case H_blank:
		ReSet_SetLow();
		H.dis_reset = true;
		break;
	default:
		break;
	}
}

/*
 * Switch button pressed IOC pin call-back functions
 * SW3 is input level only with no IOC functions
 */
static void sw2_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + DBOUNCE_COUNTS)) {
		buzzer_trigger(BZ1);
		dbounce = TMR3_CounterGet();
		H.show_la = !H.show_la;

		if (H.dis_reset) {
			H.dis_unblank = true;
		}

		if (SW3_Get()) {
			POS2CNT--;
		} else {
			POS2CNT++;
		}
		if (!H.show_la) {
			LA_gfx(true, false, 0);
		}
		hid_init(H_zero_blank); // reset the screen blanking counter
	}

}

static void sw4_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + DBOUNCE_COUNTS)) {
		buzzer_trigger(BZ3);
		dbounce = TMR3_CounterGet();
		H.dis_alt = !H.dis_alt;
		if (H.dis_reset) {
			H.dis_unblank = true;
		}
		hid_init(H_zero_blank); // reset the screen blanking counter
		if (!SW3_Get()) {
			hid_init(H_blank);
			if (!SW2_Get() && !SW5_Get()) { // set screen blanking period to ~10min
				TMR4_PeriodSet(HID_LONG_BLANK);
				buzzer_trigger(BZ_ON_LOW);
			} else {
				if (!H.silent) { // toggle buzzer sound
					buzzer_trigger(BZ_OFF);
				} else {
					H.silent = false;
				}
			}

		}
	}

}

static void sw5_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + DBOUNCE_COUNTS)) {
		buzzer_trigger(BZ1);
		dbounce = TMR3_CounterGet();
		H.la_mod = !H.la_mod;
		if (H.dis_reset) {
			H.dis_unblank = true;
		}
		if (SW3_Get()) {
			POS2CNT++;
		} else {
			POS2CNT--;
		}
		hid_init(H_zero_blank); // reset the screen blanking counter
	}

}

static void hid_blank_cb(uint32_t status, uintptr_t context)
{
	hid_init(H_blank);
	buzzer_trigger(BZ2);
}