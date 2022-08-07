/*
 * Control.h
 *
 *  Created on: 2022Äê7ÔÂ29ÈÕ
 *      Author: HASEE
 */

#ifndef FUNCTION_PWM_CONTROL_H_
#define FUNCTION_PWM_CONTROL_H_

#include "driverlib.h"
#include "PID.h"
#include "Encoder.h"
#include "led.h"
#include "pwm.h"

extern float angle;
extern unsigned char STOP;

void Motion_PID(void);
void Stop_Car(void);

#endif /* FUNCTION_PWM_CONTROL_H_ */
