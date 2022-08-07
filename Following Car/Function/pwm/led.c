/*
 * led.c
 *
 *  Created on: 2022年7月28日
 *      Author: HASEE
 */
#include "led.h"
#include "PID.h"

unsigned char Task = 0, Speed_Set = 30, Setting_State = 0, Inside_Round = 0;

void LED_Init(void)
{
    P6DIR |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P6OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
    P7DIR |= BIT0;
    P7OUT |= BIT0;
}

void KEY_Init(void)
{
    P2DIR &= ~BIT1;//板载按键s1设为输入
    P2REN |= BIT1;//上拉电阻，提高稳定性
    P2OUT |= BIT1;//设置初始值为高电平

    P1DIR &= ~BIT1;//板载按键s2设为输入
    P1REN |= BIT1;//上拉电阻，提高稳定性
    P1OUT |= BIT1;//设置初始值为高电平

    P3DIR &= ~BIT6;//板载按键s1设为输入
    P3REN |= BIT6;//上拉电阻，提高稳定性
    P3OUT |= BIT6;//设置初始值为高电平
}

void Beep_Init(void)
{
    P3DIR |= BIT5;
    P3OUT &= ~BIT5;
}

void Beep_On(void)
{
    static int n = 0;
    if(n < 200){
        P3OUT |= BIT5;
        n++;
    }
    else
        P3OUT &= ~BIT5;
}

void LED_Display(void)
{
    static int n = 0, Task_Last = 0, Speed_Last = 0;
    if(Setting_State == 0){
        P7OUT |= BIT0;
        if(Task_Last != Task)
            P6OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
        Task_Last = Task;
        switch(Task){
        case 1:
            P6OUT &= ~BIT1;
            break;
        case 2:
            P6OUT &= ~BIT2;
            break;
        case 3:
            P6OUT &= ~BIT3;
            break;
        case 4:
            P6OUT &= ~BIT4;
            break;
        }
    }
    else if(Setting_State == 1){
        P7OUT &= ~BIT0;
        if(Speed_Last != Speed_Set)
            P6OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
        Speed_Last = Speed_Set;
        n = Speed_Set / 10 - 2;
        switch(n){
        case 1://30cm/s
            P6OUT &= ~BIT1;
            break;
        case 2://40cm/s
            P6OUT &= ~BIT2;
            break;
        case 3://50cm/s
            P6OUT &= ~(BIT1 + BIT2);
            break;
        case 4://60cm/s
            P6OUT &= ~BIT3;
            break;
        case 5://70cm/s
            P6OUT &= ~(BIT1 + BIT3);
            break;
        case 6://80cm/s
            P6OUT &= ~(BIT2 + BIT3);
            break;
        case 7://90cm/s
            P6OUT &= ~(BIT1 + BIT2 + BIT3);
            break;
        case 8://100cm/s
            P6OUT &= ~BIT4;
            break;
        }
    }
    else if(Setting_State == 2){
        if(n != 0){
            P7OUT |= BIT0;
            P6OUT |= (BIT0 + BIT1 + BIT2 + BIT3 + BIT4);
            n = 0;
        }
        if(Inside_Round == 1)   P6OUT &= ~BIT0;
        else    P6OUT |= BIT0;
    }
}

void KEY_Scan(void)
{
    static unsigned char Key_Old = 0, Key_Count = 0;
    if(Setting_State == 2)  return;
    if(((P2IN & BIT1) == 0) && (Key_Old != 1)){
        Key_Count++;
        if(Key_Count > 4){
            Key_Old = 1;
            if(Setting_State == 0){
                Task++;
                if(Task > 4)    Task = 1;
            }
            else if(Setting_State == 1){
                Speed_Set += 10;
                if(Speed_Set > 100)    Speed_Set = 30;

            }
            Key_Count = 0;
            return;
        }
        return;
    }
    else if(((P1IN & BIT1) == 0) && (Key_Old != 2)){
        Key_Count++;
        if(Key_Count > 4){
            Key_Old = 2;
            Setting_State++;
            Key_Count = 0;
            if(Speed_Set < 50 && Setting_State == 2){
//                PID_Pos_3.KP = 1.2;
//                PID_Pos_3.KD = 0.1;
            }
            return;
        }
        return;
    }
    if(((P2IN & BIT1) != 0) && ((P1IN & BIT1) != 0)){
        Key_Old = 0;
        Key_Count = 0;
    }

}

