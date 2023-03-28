
#include "pid.h"

/*
 * converted to PI for motor control
 */
double UpdatePI(volatile SPid * const pid, double const error)
{
	double pTerm, iTerm;

	pTerm = pid->pGain * error; // calculate the proportional term
	// calculate the integral state with appropriate limiting
	pid->iState += error;

	if (pid->iState > pid->iMax) {
		pid->iState = pid->iMax;
	} else if (pid->iState < pid->iMin) {
		pid->iState = pid->iMin;
	} else {
		
	}

	iTerm = (pid->iGain * pid->iState); // calculate the integral term

	return pTerm + iTerm;
}

void ResetPI(volatile SPid * const pid)
{
	pid->dState = 0.0; // not used but cleared
	pid->iState = 0.0;
}



