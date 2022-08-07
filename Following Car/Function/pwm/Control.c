/*
 * Control.c
 *
 *  Created on: 2022Äê7ÔÂ29ÈÕ
 *      Author: HASEE
 */
#include "Control.h"

float angle = 0;
unsigned char STOP = 0;

void Motion_PID(void)
{
    static int n = 0;
    PID_Pos_3.Target = 0;
//    if(angle >= 30){
//       angle = angle - 100;
//       m++;
//    }
//    if(m > 0){
//        m++;
//        angle = angle + 10;
//        if(m > 100)  m = 0;
//    }
    Position_PID_Speed(angle, &PID_Pos_3);
//    if(Speed_Set > 60 && (angle > 10 || angle < -10) && n == 0){
//        PID_Pos_1.Target = 60 + PID_Pos_3.PID_Out;
//        PID_Pos_2.Target = 60 - PID_Pos_3.PID_Out;
//        n++;
//    }
//    else if(n > 0){
//        n++;
//        PID_Pos_1.Target = 60 + PID_Pos_3.PID_Out;
//        PID_Pos_2.Target = 60 - PID_Pos_3.PID_Out;
//        if(n > 30)  n = 0;
//    }
//    else if(Speed_Set > 60 && (angle < 10 && angle > -10) && n == 0){
//            PID_Pos_1.Target = Speed_Set + 5 + PID_Pos_3.PID_Out;
//            PID_Pos_2.Target = Speed_Set + 5 - PID_Pos_3.PID_Out;
//        }
//    else{
    PID_Pos_1.Target = Speed_Set + PID_Pos_3.PID_Out;
    PID_Pos_2.Target = Speed_Set - PID_Pos_3.PID_Out;
//    }
    if(PID_Pos_1.Target < 0)    PID_Pos_1.Target = 0;
    if(PID_Pos_2.Target < 0)    PID_Pos_2.Target = 0;
    Position_PID_Speed(Left_Speed, &PID_Pos_1);
    Position_PID_Speed(Right_Speed, &PID_Pos_2);

//    Set_PWM(PID_Pos_1.PID_Out, PID_Pos_2.PID_Out);
    Set_PWM(PID_Pos_1.PID_Out, PID_Pos_2.PID_Out);
}

void Stop_Car(void)
{
    static int n = 0, max_speed = 0;
    if(n == 0)  max_speed = Speed_Set;
    n++;
    Speed_Set = Speed_Set - max_speed / 20;
    PID_Pos_1.Target = Speed_Set;
    PID_Pos_2.Target = Speed_Set;
    Position_PID_Speed(Left_Speed, &PID_Pos_1);
    Position_PID_Speed(Right_Speed, &PID_Pos_2);
    if(n > 19)   Set_PWM(0, 0);
    else    Set_PWM(PID_Pos_1.PID_Out, PID_Pos_2.PID_Out);
}

