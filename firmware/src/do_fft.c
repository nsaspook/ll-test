#include "do_fft.h"
#include "fft.h"

static const char *build_date = __DATE__, *build_time = __TIME__;

void __delay_ms_fft(uint32_t delay)
{
	register uint32_t startCntms = _CP0_GET_COUNT();
	register uint32_t waitCntms = delay * (_XTAL_FREQ / 1000 / 2); //core timer runs at half of system clock

	while (_CP0_GET_COUNT() - startCntms < waitCntms);
}

void __delay_us_fft(uint32_t delay)
{
	register uint32_t startCnt = _CP0_GET_COUNT();
	register uint32_t waitCnt = delay * (_XTAL_FREQ / 1000000 / 2); //core timer runs at half of system clock

	while (_CP0_GET_COUNT() - startCnt < waitCnt);
}

void do_fft(bool sine)
{

	uint32_t i;
	double f;

	initFFT();

	if (sine) {
		/* generate sin wave */
		for (i = 0; i < N_FFT; i++) {
			f = sin(2 * PI2N * i);
			inB[i] = 128 + (unsigned char) (120.0 * f);
		}
	}

	/* fft */
	windowFFT(inB);
	FFT();
	powerScale(inB);
	/*
	 * clear the first few bins of noise
	 */
	inB[0] = 0;
	inB[1] = 0;
	inB[2] = 0;
	inB[3] = 0;
	inB[4] = 0;
	inB[5] = 0;
}

void do_fft_version(void)
{
	printf("\r--- %s Driver Version  %s %s %s ---\r\n", DO_FFT_ALIAS, DO_FFT_DRIVER, build_date, build_time);
}

/*
 * DC bias removal for each channel
 */
double do_fft_dc_x(double input)
{
	static double state = 0;
	static const double cutoff_frequency = fft_cutoff_freq;
	static const double gain = cutoff_frequency / (2 * M_PI * fft_sample_rate);

	double retval = input - state;
	state += gain * retval;
	return retval;
}

double do_fft_dc_y(double input)
{
	static double state = 0;
	static const double cutoff_frequency = fft_cutoff_freq;
	static const double gain = cutoff_frequency / (2 * M_PI * fft_sample_rate);

	double retval = input - state;
	state += gain * retval;
	return retval;
}

double do_fft_dc_z(double input)
{
	static double state = 0;
	static const double cutoff_frequency = fft_cutoff_freq;
	static const double gain = cutoff_frequency / (2 * M_PI * fft_sample_rate);

	double retval = input - state;
	state += gain * retval;
	return retval;
}