/* 
 * File:   hid.h
 * Author: root
 *
 * Created on April 11, 2023, 6:43 PM
 */

#ifndef HID_H
#define	HID_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "buzzer.h"

	typedef enum {
		H_init,
		H_home,
		H_run,
		H_abort,
	} H_STATE;

	typedef struct H_type {
		volatile uint32_t pacing;
		H_STATE hid_state;
		volatile bool dis_alt, show_la, la_mod;
	} H_type;
	
	extern H_type H;

#ifdef	__cplusplus
}
#endif

#endif	/* HID_H */

