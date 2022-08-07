/*
 * Encoder.h
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: HASEE
 */

#ifndef FUNCTION_PWM_ENCODER_H_
#define FUNCTION_PWM_ENCODER_H_

#include "driverlib.h"
#include "Filter.h"

extern int count1, count2;
extern float Left_Speed, Right_Speed, Total_Speed, Distance;

void Encoder_Init(void);
void Get_Speed(void);

#endif /* FUNCTION_PWM_ENCODER_H_ */
