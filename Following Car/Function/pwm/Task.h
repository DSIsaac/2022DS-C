/*
 * Task.h
 *
 *  Created on: 2022Äê7ÔÂ29ÈÕ
 *      Author: HASEE
 */

#ifndef FUNCTION_PWM_TASK_H_
#define FUNCTION_PWM_TASK_H_

#include "driverlib.h"
#include "pwm.h"
#include "usart.h"
#include "time.h"
#include "adc.h"
#include "icm.h"
#include "IMU.h"
#include "Filter.h"
#include "led.h"
#include "Encoder.h"
#include "PID.h"
#include "Control.h"

extern unsigned char STOP_Flag;

void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);

#endif /* FUNCTION_PWM_TASK_H_ */
