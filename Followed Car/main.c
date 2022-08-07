#include "driverlib.h"
#include "pwm.h"
#include "usart.h"
#include "time.h"
#include "adc.h"
#include "icm.h"
#include "IMU.h"
#include "Filter.h"
#include "led.h"
#include "Encoder.h"
#include "PID.h"
#include "Control.h"
#include "Task.h"

void main(void)
{
    PMM_setVCore(PMM_CORE_LEVEL_3);
    WDT_A_hold(WDT_A_BASE);                   //Stop WDT
    usart_init();
    usart0_init();
    rtc_init();
    time_init();
    LED_Init();
    Beep_Init();
    KEY_Init();
    PWM_Init(10000);
    Encoder_Init();
    PID_Init();
    Send_To_Openmv(Inside_Round, 0);
    while(uart0_recieve[0] != 0xae);
    while(Setting_State != 2){
        if(time20ms >= 20){
            time20ms = 0;
            KEY_Scan();
        }
        if(time100ms >= 100){
            time100ms = 0;
            LED_Display();
        }
    }
    switch(Task){
        case 1:
            Task1();
            break;
        case 2:
            Speed_Set = 50;
            Task2();
            break;
        case 3:
            Task3();
            break;
        case 4:
            Task4();
            break;
    }
 }
