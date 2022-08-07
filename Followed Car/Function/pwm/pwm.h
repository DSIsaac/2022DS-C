#ifndef pwm_h
#define pwm_h

#include "driverlib.h"

extern unsigned char DIR1, DIR2;//0 for forward and 1 for backward

void PWM_Init(int TIMER_PERIOD);
void Set_PWM(int pwm1, int pwm2);
void set_pwm1(int duty);
void set_pwm2(int duty);
void set_pwm3(int duty);
void set_pwm4(int duty);

#endif

