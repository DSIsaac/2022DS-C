/*
 * led.h
 *
 *  Created on: 2022Äê7ÔÂ28ÈÕ
 *      Author: HASEE
 */

#ifndef FUNCTION_PWM_LED_H_
#define FUNCTION_PWM_LED_H_

#include "driverlib.h"
#include "msp430.h"

extern unsigned char Task, Speed_Set, Setting_State, Inside_Round;

void LED_Init(void);
void KEY_Init(void);
void Beep_Init(void);
void Beep_On(void);
void LED_Display(void);
void KEY_Scan(void);

#endif /* FUNCTION_PWM_LED_H_ */
