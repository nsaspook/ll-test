/*
 * Internal driven magnetic buzzer sounds ISR and PWM handler FSM
 * loudness: Attack slope, sustain slop, decay slope and timing
 * to adjust the dynamic sound character of each buzzer trigger
 * 
 */


#include "buzzer.h"
#include "hid.h"

static const uint32_t vol_high = 10000, vol_mute = 0, vol_low = 2000;

B_type tone1 = {
	.arate = 15,
	.alimit = 7000,
	.srate = 10,
	.slimit = 8000,
	.drate = 30,
	.dlimit = 500,
};

B_type tone2 = {
	.arate = 500,
	.alimit = 7000,
	.srate = 10,
	.slimit = 10000,
	.drate = 30,
	.dlimit = 500,
};

B_type tone3 = {
	.arate = 500,
	.alimit = 7000,
	.srate = 1,
	.slimit = 9000,
	.drate = 5,
	.dlimit = 500,
};

volatile uint32_t buzzer_state = 0;

static void buzzer_tone(uint32_t, uintptr_t);

/*
 * setup and start the needed hardware and software
 */
void buzzer_init(B_STATE bs)
{
	switch (bs) {
	case B_abort:
		MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, vol_mute);
		TMR2_Stop();
		MCPWM_Stop();
		break;
	case B_init:
	default:
		TMR2_CallbackRegister(buzzer_tone, 0);
		TMR2_Start();

		MCPWM_Start();
		MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, vol_mute);
		break;
	}
}

/*
 * buzzer amplitude/tone profiles
 * this gets called every 10us in the timer TMR2 callback interrupt
 */
static void buzzer_tone(uint32_t status, uintptr_t context)
{
	static uint32_t volume = vol_mute;

	switch (buzzer_state) {
	case BZ0:
		volume = vol_mute;
		break;
	case BZ_ON_LOUD: // steady loud
		volume = vol_high;
		break;
	case BZ_ON_LOW: // steady low
		volume = vol_low;
		break;
	case BZ1: // short chirp
		volume += tone1.arate;
		if (volume >= tone1.alimit) {
			buzzer_state = BZ1 + 1;
		}
		break;
	case BZ1 + 1:
		volume += tone1.srate;
		if (volume >= tone1.slimit) {
			buzzer_state = BZ1 + 2;
		}
		break;
	case BZ1 + 2:
		volume -= tone1.drate;
		if (volume <= tone1.dlimit) {
			volume = vol_mute;
			buzzer_state = BZ0;
		}
		break;
	case BZ2: // short click
		volume += tone2.arate;
		if (volume >= tone2.alimit) {
			buzzer_state = BZ2 + 1;
		}
		break;
	case BZ2 + 1:
		volume += tone2.srate;
		if (volume >= tone2.dlimit) {
			buzzer_state = BZ2 + 2;
		}
		break;
	case BZ2 + 2:
		volume -= tone2.drate;
		if (volume <= tone2.dlimit) {
			volume = vol_mute;
			;
			buzzer_state = BZ0;
		}
		break;
	case BZ3: // longer click
		volume += tone3.arate;
		if (volume >= tone3.dlimit) {
			buzzer_state = BZ3 + 1;
		}
		break;
	case BZ3 + 1:
		volume += tone3.srate;
		if (volume >= tone3.slimit) {
			buzzer_state = BZ3 + 2;
		}
		break;
	case BZ3 + 2:
		volume -= tone3.drate;
		if (volume <= tone3.dlimit) {
			volume = vol_mute;
			buzzer_state = BZ0;
		}
		break;
	case BZ_OFF:
		H.silent = true;
		break;
	default:
		buzzer_state = BZ0;
		volume = vol_mute;
		break;
	}

	if (H.silent) {
		MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, vol_mute);
	} else {
		MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, volume);
	}
	if (volume == vol_mute) { // stop callback interrupts
		TMR2_Stop();
	}
}

void buzzer_trigger(uint32_t trigger)
{
	TMR2_Start(); // start callback interrupts to process buzzer profiles
	buzzer_state = trigger;
}

uint32_t get_buzzer_trigger(void)
{
	return buzzer_state;
}