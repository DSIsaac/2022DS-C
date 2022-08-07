#include "time.h"

void rtc_init(void)
{
  /*P5SEL |= BIT2+BIT3;                       

  UCSCTL6 &= ~XT2OFF;                       //xt2  4mhz
  UCSCTL3 |= SELREF_2;                      

  UCSCTL4 |= SELA_2;                       // ACLK=REFO  32.768khz ,SMCLK=DCO,MCLK=DCO
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);                                      
    SFRIFG1 &= ~OFIFG;                      
  }while (SFRIFG1&OFIFG);                 

  UCSCTL6 &= ~XT2DRIVE0;                        
  UCSCTL4 |= SELS_5 + SELM_5;   */
  
  UCSCTL3 |= SELREF_2;                     
  UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

  __bis_SR_register(SCG0);                 
  UCSCTL0 = 0x0000;                         
  UCSCTL1 = DCORSEL_7;                      // Select DCO range 24MHz operation
  UCSCTL2 = FLLD_0 + 762;                   // Set DCO Multiplier for 12MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (374 + 1) * 32768 = 12MHz
                                            // Set FLL Div = fDCOCLK/2
  __bic_SR_register(SCG0);                  // Enable the FLL control loop

  __delay_cycles(782000);
	
  do
  {
    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
                                            // Clear XT2,XT1,DCO fault flags
    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  }while (SFRIFG1&OFIFG); 
  
}


void time_init(void)
{
    TA1R = 0;
    TA1CCTL0 = CCIE;
    TA1CCR0 = 25000 - 1;
    TA1CTL = TASSEL_2 + MC_1 + TACLR;       //smclk
    __enable_interrupt();
}
 
unsigned int time1ms = 0, time2ms = 0, time5ms = 0, time10ms = 0, time20ms = 0, time100ms = 0, time1s;

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timea(void)
 {
    time1ms++;
    time2ms++;
    time5ms++;
    time10ms++;
    time20ms++;
    time100ms++;
    time1s++;
 }


