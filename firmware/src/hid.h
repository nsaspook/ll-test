/* 
 * TMR3 debounce timer
 * TMR4 screen blank timer
 */

#ifndef HID_H
#define	HID_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "buzzer.h"
#include "gfx.h"

#define HID_DRIVER "V1.015" 
#define HID_ALIAS "HID"

#define FFT_WAKE	1500
#define HID_LONG_BLANK	14062499U * 10U

#define DBOUNCE_COUNTS	70000
	/*
	 * silent mode animation
	 */
#define SDOT_ON		70
#define SDOT_OFF	140

#define PROX_T_H	8.5f
#define PROX_T_L	1.0f

	typedef enum {
		H_init,
		H_home,
		H_run,
		H_blank,
		H_zero_blank,
		H_abort,
	} H_STATE;

	typedef struct H_type {
		H_STATE hid_state;
		volatile bool dis_alt, show_la, la_mod, dis_unblank, dis_reset, silent;
	} H_type;

	extern H_type H;

	extern void hid_init(H_STATE);

#ifdef	__cplusplus
}
#endif

#endif	/* HID_H */

