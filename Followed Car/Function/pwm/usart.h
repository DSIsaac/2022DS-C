#ifndef usart_h
#define usart_h

#include "driverlib.h"
#include "Control.h"

void send_data(unsigned char data);
void usart_init(void);
void usart0_init(void);
void send_data_uart0(unsigned char data);
void sendSenser(int a_x,int a_y,int a_z,unsigned char _fu);
void Send_To_Openmv(unsigned char round, unsigned char stop_symbol);
void Send_To_Following_Car(unsigned char task, unsigned char stop_symbol);

extern unsigned char uart0_recieve[6];

#endif
