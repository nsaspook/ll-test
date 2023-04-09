#include "buzzer.h"

volatile uint32_t buzzer_state = 0;

void buzzer_init(void)
{
	TMR2_CallbackRegister(buzzer_tone, 0);
	TMR2_Start();

	MCPWM_Start();
	MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, 0);
}

void buzzer_tone(uint32_t status, uintptr_t context)
{
	static uint32_t volume = 0;

	switch (buzzer_state) {
	case 0:
		volume = 0;
		break;
	case 1:
		volume += 15;
		if (volume >= 7000) {
			buzzer_state = 2;
		}
		break;
	case 2:
		volume += 10;
		if (volume >= 8000) {
			buzzer_state = 3;
		}
		break;
	case 3:
		volume -= 30;
		if (volume <= 500) {
			volume = 0;
			buzzer_state = 0;
		}
		break;
	default:
		buzzer_state = 0;
		volume = 0;
		break;
	}
	MCPWM_ChannelPrimaryDutySet(MCPWM_CH_1, volume);
}

void buzzer_trigger(uint32_t trigger)
{
	buzzer_state = trigger;
}
