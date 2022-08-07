/*
 * Task.c
 *
 *  Created on: 2022Äê7ÔÂ29ÈÕ
 *      Author: HASEE
 */
#include "Task.h"

unsigned char STOP_Flag = 0;

void Task1(void)
{
    PID_Pos_3.KP = 1.5;
    PID_Pos_3.KD = 0.1;
    while(1){
        if(time10ms >= 10){
            time10ms = 0;
            Get_Speed();
//            STOP = STOP_Flag;
            if(STOP < 1)
                Motion_PID();
            else if(STOP >= 1){
                STOP_Flag = 1;
                Stop_Car();
                Beep_On();
            }
            else{
                STOP = 1;
                STOP_Flag = 1;
                Stop_Car();
                Beep_On();
            }
            Send_To_Openmv(Inside_Round, 0);
            Send_To_Following_Car(1, STOP);

        }
        if(time100ms >= 100){
            time100ms = 0;
            LED_Display();
        }
        if(time1s >= 1000){
            time1s = 0;
            time1ms = 0;
            time2ms = 0;
            time5ms = 0;
            time20ms = 0;
        }
    }
}

void Task2(void)
{
//    int n = 0;
    PID_Pos_3.KP = 2.0;
   PID_Pos_3.KD = 0.18;
    while(1){
//        n = STOP;
        if(time10ms >= 10){
            time10ms = 0;
            Get_Speed();
//            STOP = STOP_Flag;
            if(STOP < 2){
                Motion_PID();
                Send_To_Openmv(0, 0);
                Send_To_Following_Car(2, 0);
            }
            else if(STOP >= 2){
                Stop_Car();
                Beep_On();
                Send_To_Following_Car(2, 1);
            }


        }
        if(time100ms >= 100){
            time100ms = 0;
            LED_Display();
        }
        if(time1s >= 1000){
            time1s = 0;
            time1ms = 0;
            time2ms = 0;
            time5ms = 0;
            time20ms = 0;
        }
    }
}

void Task3(void)
{
    static int n = 0;
    Speed_Set = 50;
    PID_Pos_3.KP = 2.0;
    PID_Pos_3.KD = 0.18;
    while(1){
        if(time10ms >= 10){
            time10ms = 0;
            Get_Speed();
            if(STOP < 3){
                if(STOP < 2)    Inside_Round = 0;
                else if(STOP >= 2 && n == 0)  Inside_Round = 1;
                if(Inside_Round == 1){
                    n++;
                    if(n > 400){
//                        n = 0;
                        Inside_Round = 0;
                    }
                }
                Motion_PID();
                Send_To_Openmv(Inside_Round, 0);
                Send_To_Following_Car(3, STOP);
            }
            else if(STOP >= 3){
                Stop_Car();
                Beep_On();
                Send_To_Following_Car(3, STOP);
            }


        }
        if(time100ms >= 100){
            time100ms = 0;
            LED_Display();
        }
        if(time1s >= 1000){
            time1s = 0;
            time1ms = 0;
            time2ms = 0;
            time5ms = 0;
            time20ms = 0;
        }
    }
}

void Task4(void)
{
    static int n = 0;
    Speed_Set = 55;
    PID_Pos_3.KP = 2.0;
    PID_Pos_3.KD = 0.18;
    while(1){
        if(time10ms >= 10){
            time10ms = 0;
            Get_Speed();
            if(STOP == 0 || (STOP == 1 && n > 499))
                Motion_PID();
            else if(STOP == 1 && n < 500){
                n++;
                Stop_Car_Task4();
                PID_Pos_1.Integral = 0;
                PID_Pos_2.Integral = 0;
                PID_Pos_1.Error = 0;
                PID_Pos_2.Error = 0;
                PID_Pos_1.Last_Error = 0;
                PID_Pos_2.Last_Error = 0;
            }
            else if(STOP == 2){
                Stop_Car();
                Beep_On();
            }
            Send_To_Openmv(Inside_Round, 0);
            Send_To_Following_Car(4, STOP);

        }
        if(time100ms >= 100){
            time100ms = 0;
            LED_Display();
        }
        if(time1s >= 1000){
            time1s = 0;
            time1ms = 0;
            time2ms = 0;
            time5ms = 0;
            time20ms = 0;
        }
    }
}
