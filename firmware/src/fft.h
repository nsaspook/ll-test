/* 
 * File:   fft.h
 * Author: root
 *
 * Created on November 6, 2022, 1:51 PM
 */

#ifndef FFT_H
#define	FFT_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "imupic32mcj.h"

#define FFT_DRIVER "V1.160" 
#define FFT_ALIAS "FFT"

#define N_FFT 256
#define PI2N 2 * M_PI / N_FFT
#define FFT_COUNT   512

#ifdef BMA400 // Bosch devices, gain and noise product
#define FFT_GAIN	5.0  // low cost, high noise
#else
#define FFT_GAIN	40.0 // Murata imu
#endif

	extern uint8_t inB[], fft_buffer[];
	extern double fft_gain;

	void initFFT(void);
	void windowFFT(uint8_t *source);
	void FFT(void);
	void powerScale(uint8_t *dest);
	void fft_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FFT_H */

