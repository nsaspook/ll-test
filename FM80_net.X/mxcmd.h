/* 
 * File:   mxcmd.h
 * Author: root
 *
 * Created on June 2, 2023, 6:16 AM
 */

#ifndef MXCMD_H
#define	MXCMD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"

#define	FM_BUFFER	32

	const uint16_t cmd_id[] = {0x100, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
	const uint16_t cmd_status[] = {0x100, 0x02, 0x01, 0xc8, 0x00, 0x00, 0x00, 0xcb};
	const uint16_t cmd_panelv[] = {0x100, 0x02, 0x01, 0xc6, 0x00, 0x00, 0x00, 0xc9};
	const uint16_t cmd_batteryv[] = {0x100, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0a};
	const uint16_t cmd_watts[] = {0x100, 0x02, 0x00, 0x15, 0x00, 0x00, 0x00, 0x17};
	const uint16_t cmd_misc[] = {0x100, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}; // example MX80 command request ID

	enum status_type {
		STATUS_SLEEPING = 0,
		STATUS_FLOATING = 1,
		STATUS_BULK = 2,
		STATUS_ABSORB = 3,
		STATUS_EQUALIZE = 4,
		STATUS_LAST,
	};
	
	const char state_name [][12] = {
	"Sleeping",
	"Floating",
	"Bulk",
	"Absorb",
	"Equalize",
	"Last",
};

	extern void onesec_io(void);
	extern void FM_io(void);
	extern uint8_t FM_tx(const uint16_t *, uint8_t);
	extern bool FM_tx_empty(void);
	extern uint8_t FM_rx(uint16_t *);
	extern bool FM_rx_ready(void);
	extern uint8_t FM_rx_count(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MXCMD_H */

