#ifndef spi_h
#define spi_h

#include "driverlib.h"


#define MAX_X   128
#define MAX_Y   64
#define SIZE    16

#define OLED_CS(x)  x?( GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN0) ):(GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN0))
#define OLED_DC(x)  x?( GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN1) ):(GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN1))
#define OLED_RES(x) x?( GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN2) ):(GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN2))
#define OLED_D1(x)  x?( GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN3) ):(GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN3))
#define OLED_SCK(x)  x?( GPIO_setOutputHighOnPin(GPIO_PORT_P6,GPIO_PIN4) ):(GPIO_setOutputLowOnPin(GPIO_PORT_P6,GPIO_PIN4))

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1 	//Ð´Êý¾Ý
#define uint8_t   unsigned char
#define u8   uint8_t
#define u32 unsigned int

unsigned char spi_send(unsigned char dat );
void spi_init(void);
void OLED_Wr_Byte(uint8_t dat, uint8_t cmd);
void OLED_OFF(void);
void OLED_ON(void);
void OLED_Set_Position(uint8_t x, uint8_t y);
void OLED_Init(void);
void OLED_Show(uint8_t x,uint8_t y,uint8_t no);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t no);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_Clear_Block(uint8_t x, uint8_t y);
void OLED_Clear_Line(uint8_t y);
void show_big_num(uint8_t x,uint8_t y,uint8_t num);


#endif
