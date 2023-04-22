/* 
 * File:   bno086.h
 * Author: root
 *
 * Created on April 20, 2023, 11:51 AM
 */

#ifndef BNO086_H
#define	BNO086_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <math.h>
#include "definitions.h"                // SYS function prototypes
#include "imupic32mcj.h"
#include "bno086_reg.h"
#include "imu.h"

#define BNO086_DRIVER	"V1.000" 
#define BNO086_ALIAS	"BNO086  "

	void bno086_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BNO086_H */

