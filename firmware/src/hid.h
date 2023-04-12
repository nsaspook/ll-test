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
#include "gfx.h"

	typedef enum {
		H_init,
		H_home,
		H_run,
		H_blank,
		H_zero_blank,
		H_abort,
	} H_STATE;

	typedef struct H_type {
		volatile uint32_t pacing;
		H_STATE hid_state;
		volatile bool dis_alt, show_la, la_mod, dis_unblank, dis_reset;
	} H_type;

	extern H_type H;

	extern void hid_init(H_STATE);

	void sw2_cb(GPIO_PIN, uintptr_t);
	void sw4_cb(GPIO_PIN, uintptr_t);
	void sw5_cb(GPIO_PIN, uintptr_t);

#ifdef	__cplusplus
}
#endif

#endif	/* HID_H */

