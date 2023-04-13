/* 
 * File:   buzzer.h
 * Author: root
 *
 * Created on April 9, 2023, 10:48 AM
 */

#ifndef BUZZER_H
#define	BUZZER_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "imupic32mcj.h"

#define BUZZER_DRIVER "V1.000" 
#define BUZZER_ALIAS "BUZZER"
	
#define BZ0	0
#define BZ1	1
#define BZ2	11
#define BZ3	21
	
#define BZ_ON_LOUD	100
#define BZ_ON_LOW	101

	void buzzer_init(void);
	void buzzer_tone(uint32_t, uintptr_t);
	void buzzer_trigger(uint32_t);

#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */

