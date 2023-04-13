
#include "hid.h"

H_type H = {
	.dis_alt = false,
	.dis_unblank = false,
	.show_la = true,
	.la_mod = true,
	.dis_reset = false,
	.pacing = 0,
	.hid_state = H_init,
};

void hid_blank_cb(uint32_t, uintptr_t);

void hid_init(H_STATE hs)
{
	switch (hs) {
	case H_init:
		TMR4_CallbackRegister(hid_blank_cb, 0);
		TMR4_Start();
		break;
	case H_zero_blank:
		TMR4 = 0x0;
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
 * Switch button pressed call-back functions
 * SW3 is input level only with no IOC functions
 */
void sw2_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + 70000)) {
		buzzer_trigger(1);
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

void sw4_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + 70000)) {
		buzzer_trigger(1);
		dbounce = TMR3_CounterGet();
		H.dis_alt = !H.dis_alt;
		hid_init(H_zero_blank); // reset the screen blanking counter
	}

}

void sw5_cb(GPIO_PIN pin, uintptr_t context)
{
	static uint32_t dbounce = 0;

	if (TMR3_CounterGet() > (dbounce + 70000)) {
		buzzer_trigger(1);
		dbounce = TMR3_CounterGet();
		H.la_mod = !H.la_mod;
		if (SW3_Get()) {
			POS2CNT++;
		} else {
			POS2CNT--;
		}
		hid_init(H_zero_blank); // reset the screen blanking counter
	}

}

void hid_blank_cb(uint32_t status, uintptr_t context)
{
	hid_init(H_blank);
}