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

void buzzer_init(void);
void buzzer_tone(uint32_t, uintptr_t);
void buzzer_trigger(uint32_t);

#ifdef	__cplusplus
}
#endif

#endif	/* BUZZER_H */

