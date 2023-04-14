/*
 * TMR2 buzzer PWM update timer
 */

#ifndef BUZZER_H
#define	BUZZER_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "imupic32mcj.h"

#define BUZZER_DRIVER "V1.002" 
#define BUZZER_ALIAS "BUZZER"

#define BZ0	0
#define BZ1	1
#define BZ2	11
#define BZ3	21

#define BZ_OFF	255

#define BZ_ON_LOUD	100
#define BZ_ON_LOW	101

	typedef enum {
		B_init,
		B_abort,
	} B_STATE;

	typedef struct B_type {
		volatile uint32_t arate, alimit;
		volatile uint32_t srate, slimit;
		volatile uint32_t drate, dlimit;
	} B_type;

	void buzzer_init(B_STATE);
	void buzzer_trigger(uint32_t);
	uint32_t get_buzzer_trigger(void);

	extern B_type tone1, tone2, tone3;

#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */

