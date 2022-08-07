/*
 * PID.h
 *
 *  Created on: 2022Äê7ÔÂ29ÈÕ
 *      Author: HASEE
 */

#ifndef FUNCTION_PWM_PID_H_
#define FUNCTION_PWM_PID_H_
#include "driverlib.h"
typedef struct
{
    float KP;
    float KI;
    float KD;
    float Target;
    int INT_UP;
    int INT_DOWN;
    float Error;
    float Last_Error;
    float Pre_Error;
    float Integral;
    float PID_Out;
}_PID_Position;
extern _PID_Position PID_Pos_1;
extern _PID_Position PID_Pos_2;
extern _PID_Position PID_Pos_3;
extern _PID_Position PID_Pos_4;

void PID_Init(void);
void Position_PID_Speed(int Value, _PID_Position *PID_Pos_n);

#endif /* FUNCTION_PWM_PID_H_ */
