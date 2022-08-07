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
            if(STOP == 0)
               Motion_PID();
            else if(STOP >= 1){
               Stop_Car();
               Beep_On();
            }
            Send_To_Openmv(Inside_Round, 0);
        }
        if(time100ms >= 100){
           time100ms = 0;
           LED_Display();
           if((P3IN & BIT6) == 0){
               Speed_Set -= 3;
               if(Speed_Set < 18)   Speed_Set = 18;
           }
           else{
               Speed_Set += 1;
               if(Speed_Set > 30)  Speed_Set = 30;
           }

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
    static char Carry_Up = 0, n = 0;
    while(1){
        if(time10ms >= 10){
            time10ms = 0;
            Get_Speed();
            if(STOP == 0)
                Motion_PID();
            else if(STOP == 1){
               Stop_Car();
               Beep_On();
            }
            Send_To_Openmv(Inside_Round, 0);

        }
        if(time20ms >= 20){
            KEY_Scan();
        }
        if(time100ms >= 100){
            time100ms = 0;
            LED_Display();
            if((P3IN & BIT6) == 0 && Speed_Set == 60){
               Speed_Set = 50;
            }
            else if((P3IN & BIT6) == 0 && Speed_Set < 60 && Speed_Set > 40){
                Speed_Set -= 3;
                if(Speed_Set < 40)   Speed_Set = 40;
                Carry_Up = 1;
            }
            else if(Carry_Up == 1){
                if(n > 2){
                    n = 0;
                    Speed_Set++;
                    if(Speed_Set > 50)  Speed_Set = 50;
                }
                n++;

            }
        }
        if(time1s >= 1000){
            time1s = 0;
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
            if(STOP < 3)
               Motion_PID();
            else if(STOP == 3){
               Stop_Car();
               Beep_On();
            }
            if(STOP == 1){
                Inside_Round = 1;
                n++;
                if(n < 400) Speed_Set = 55;
                else{
                    Speed_Set = 50;
                    Inside_Round = 0;
                }
            }
            else    Inside_Round = 0;
            Send_To_Openmv(Inside_Round, 0);
        }
        if(time100ms >= 100){
           time100ms = 0;
           LED_Display();
           if((P3IN & BIT6) == 0){
               Speed_Set -= 1;
               if(Speed_Set < 40)   Speed_Set = 40;
           }
           else{
               Speed_Set += 1;
               if(Speed_Set >= 50)  Speed_Set = 50;
           }

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
    Speed_Set = 30;
    while(1){
        if(time10ms >= 10){
            time10ms = 0;
            Get_Speed();
            if(STOP < 2)
               Motion_PID();
            else if(STOP >= 2){
               Stop_Car();
               Beep_On();
            }
            Send_To_Openmv(0, 0);
        }
        if(time100ms >= 100){
           time100ms = 0;
           LED_Display();
           if((P3IN & BIT6) == 0){
               Speed_Set -= 3;
//               if(Speed_Set < 30)   Speed_Set = 30;
           }
           else{
               Speed_Set += 1;
               if(Speed_Set > 30)  Speed_Set = 30;
           }

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
