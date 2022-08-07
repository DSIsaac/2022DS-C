#include "Encoder.h"

int count1 = 0, count2 = 0;
float Left_Speed = 0, Right_Speed = 0, Total_Speed = 0;
float Distance = 0;
char dir1 = 1, dir2 = 1;
void Encoder_Init(void)
{
    P1OUT &= ~(BIT2 + BIT3);
    P1IE |= (BIT2 + BIT3);
    P1IES &= ~(BIT2 + BIT3);
    P1OUT &= ~(BIT2 + BIT3);
    P1REN |= (BIT2 + BIT3);

//    P1OUT &= ~(BIT2);
//    P1IE |= (BIT2);
//    P1IES &= ~(BIT2);
//    P1OUT &= ~(BIT2);
//    P1REN |= (BIT2);

  __enable_interrupt();
}

#pragma vector=PORT1_VECTOR //固定格式，声明中断向量地址
__interrupt void Port1_ISR(void) {//中断子程序
//    if(P1IN & BIT2)
//        dir1 = 1;
//    else
//        dir1 = 0;
//    if((P1IN & BIT2) && ((P1IN & BIT3) == 0)){
//        count1++;
//        P1IFG &= ~BIT2;
//    }
//    else if(((P1IN & BIT2) == 0) && (P1IN & BIT3)){
//        count2++;
//        P1IFG &= ~BIT3;
//    }
//    else if((P1IN & BIT2) && (P1IN & BIT3)){
//        count1++;
//        P1IFG &= ~BIT2;
//        count2++;
//        P1IFG &= ~BIT3;
//    }
    if(P1IN & BIT2 && P1IFG & BIT2){
            count1++;
            P1IFG &= ~BIT2;
        }
    if(P1IN & BIT3 && P1IFG & BIT3){
        count2++;
        P1IFG &= ~BIT3;
    }
}

void Get_Speed(void)
{
//    acc.x = count1;
//    acc.y = count2;
//    ACC_IIR_Filter(&acc, &filter_acc, &Butter1);
//    Left_Speed = (float)filter_acc.x * 3.1415926535895f / 1500 * 71;
//    Right_Speed = (float)filter_acc.y * 3.1415926535895f / 1500 * 71;
    Left_Speed = count1 * 3.1415926535895f / 1500 * 71;
    Right_Speed = count2 * 3.1415926535895f / 1500 * 71;
    Total_Speed = (Left_Speed + Right_Speed) / 2;
    Distance += Total_Speed * 0.01f;
    count1 = 0;
    count2 = 0;
}

//#pragma vector=PORT2_VECTOR //固定格式，声明中断向量地址
//__interrupt void Port2_ISR(void) {//中断子程序
////    if(P1IN & BIT3)
////        dir2 = 1;
////    else
////        dir2 = 0;
//
//    count2++;
//    P1IFG &= ~BIT3;
//}
