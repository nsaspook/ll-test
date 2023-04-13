#include "buzzer.h"

static const uint32_t vol_high = 10000, vol_mute = 0, vol_low = 2000;

volatile uint32_t buzzer_state = 0;

void buzzer_init(void)
{
	TMR2_CallbackRegister(buzzer_tone, 0);
	TMR2_Start();

	MCPWM_Start();
	MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, 0);
}

/*
 * buzzer amplitude/tone profiles
 * this gets called every 10us in the timer TMR2 callback interrupt
 */
void buzzer_tone(uint32_t status, uintptr_t context)
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
		volume += 15;
		if (volume >= 7000) {
			buzzer_state = BZ1 + 1;
		}
		break;
	case BZ1 + 1:
		volume += 10;
		if (volume >= 8000) {
			buzzer_state = BZ1 + 2;
		}
		break;
	case BZ1 + 2:
		volume -= 30;
		if (volume <= 500) {
			volume = vol_mute;
			buzzer_state = BZ0;
		}
		break;
	case BZ2: // short click
		volume += 500;
		if (volume >= 7000) {
			buzzer_state = BZ2 + 1;
		}
		break;
	case BZ2 + 1:
		volume += 10;
		if (volume >= vol_high) {
			buzzer_state = BZ2 + 2;
		}
		break;
	case BZ2 + 2:
		volume -= 30;
		if (volume <= 500) {
			volume = vol_mute;
			;
			buzzer_state = BZ0;
		}
		break;
	case BZ3: // longer click
		volume += 500;
		if (volume >= 7000) {
			buzzer_state = BZ3 + 1;
		}
		break;
	case BZ3 + 1:
		volume += 1;
		if (volume >= 9000) {
			buzzer_state = BZ3 + 2;
		}
		break;
	case BZ3 + 2:
		volume -= 5;
		if (volume <= 500) {
			volume = vol_mute;
			buzzer_state = BZ0;
		}
		break;
	default:
		buzzer_state = BZ0;
		volume = vol_mute;
		break;
	}
	MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, volume);
	if (volume == vol_mute) { // stop callback interrupts
		TMR2_Stop();
	}
}

void buzzer_trigger(uint32_t trigger)
{
	TMR2_Start(); // start callback interrupts to process buzzer profiles
	buzzer_state = trigger;
}
