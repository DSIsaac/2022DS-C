#include "pwm.h"

unsigned char DIR1 = 0, DIR2 = 0;//0 for forward and 1 for backward

void PWM_Init(int TIMER_PERIOD)
{

    P1DIR |= BIT4 + BIT5;                       // P2.0 and P2.1 output
    P1SEL |= BIT4 + BIT5;                       // P2.0 and P2.1 options select
    P2DIR |= BIT4 + BIT5;                       // P2.0 and P2.1 output
    P2SEL |= BIT4 + BIT5;                       // P2.0 and P2.1 options select
    TA0CCR0 = TIMER_PERIOD - 1;                          // PWM Period
    TA2CCR0 = TIMER_PERIOD - 1;                         // PWM Period

    TA0CCTL3 = OUTMOD_7;                      // CCR1 reset/set
    TA0CCR3 = 0;                            // CCR1 PWM duty cycle
    TA0CCTL4 = OUTMOD_7;                      // CCR2 reset/set
    TA0CCR4 = 0;                            // CCR2 PWM duty cycle

    TA2CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TA2CCR1 = 0;                            // CCR1 PWM duty cycle
    TA2CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TA2CCR2 = 0;                            // CCR2 PWM duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TACLR;         // ACLK, up mode, clear TAR
    TA2CTL = TASSEL_2 + MC_1 + TACLR;

    P3DIR |= BIT7;
    P3OUT &= ~BIT7;
    P8DIR |= BIT2;
    P8OUT &= ~BIT2;

    P4DIR |= BIT0 + BIT3;
    P4OUT &= ~(BIT0 + BIT3);

//    Set_PWM(800, 800);
}

void Set_PWM(int pwm1, int pwm2)
{
    if(pwm1 > 0){
        P3OUT &= ~BIT7;
        P8OUT |= BIT2;
        DIR1 = 0;
    }
    else if(pwm1 < 0){
        P3OUT |= BIT7;
        P8OUT &= ~BIT2;
        DIR1 = 1;
    }
//
    if(pwm2 > 0){
        P4OUT &= ~BIT0;
        P4OUT |= BIT3;
        DIR2 = 0;
    }
    else if(pwm2 < 0){
        P4OUT |= BIT0;
        P4OUT &= ~BIT3;
        DIR2 = 1;
    }
    TA2CCR1 = pwm1;//A
    TA2CCR2 = pwm1;//C
    TA0CCR3 = pwm2;//B
    TA0CCR4 = pwm2;//D


}

void set_pwm1(int duty)
{
    TA0CCR3 = duty;
}
void set_pwm2(int duty)
{
    TA0CCR4 = duty;
}
void set_pwm3(int duty)
{
    TA2CCR1 = duty;
}
void set_pwm4(int duty)
{
    TA2CCR2 = duty;
}
