/* 
 * File:   do_fft.h
 * Author: root
 *
 * Created on November 6, 2022, 1:53 PM
 */

#ifndef DO_FFT_H
#define	DO_FFT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <p32xxxx.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
	
#include "fft.h"

#define _XTAL_FREQ 120000000UL
	
#define DO_FFT_DRIVER	"V1.002" 
#define DO_FFT_ALIAS	"DO_FFT  "
	
#define fft_sample_rate	100.0
#define fft_cutoff_freq	0.5
		
void do_fft(bool);
void __delay_ms_fft(uint32_t);
void __delay_us_fft(uint32_t);
void do_fft_version(void);

double do_fft_dc_x(double input);
double do_fft_dc_y(double input);
double do_fft_dc_z(double input);

extern uint8_t inB[N_FFT];

#ifdef	__cplusplus
}
#endif

#endif	/* DO_FFT_H */

