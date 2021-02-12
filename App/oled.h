#ifndef __K60_OLED_H__
#define __K60_OLED_H__
//#include "K60_datatype.h"

#define byte  char
#define uint unsigned int

#define SCL_PIN PORTC,5
#define SDA_PIN PORTC,7
#define RST_PIN PORTC,9
#define DC_PIN PORTC,11

//各GPIO口统一名称:PORT_A、 PORT_B、 PORT_C、 PORT_D、PORT_E
#define PORT_A   0      
#define PORT_B   1
#define PORT_C   2
#define PORT_D   3
#define PORT_E   4


#define INIT 0

#define LED_SCL_Init  gpio_init(PTI3,GPO,INIT) // 时钟初始化定义

#define LED_SDA_Init  gpio_init(PTI4,GPO,INIT)//数据口D0

#define LED_RST_Init  gpio_init(PTI5,GPO,INIT)//复位低能电平

#define LED_DC_Init   gpio_init(PTI6,GPO,INIT)//偏置常低

#define LED_SCLH  gpio_set(PTI3,1)// 时钟定义 /////注意这里
#define LED_SCLL  gpio_set(PTI3,0)

#define LED_SDAH  gpio_set(PTI4,1)//数据口D0
#define LED_SDAL  gpio_set(PTI4,0)

#define LED_RSTH  gpio_set(PTI5,1)//复位低能电平
#define LED_RSTL  gpio_set(PTI5,0)

#define LED_DCH   gpio_set(PTI6,1)
#define LED_DCL   gpio_set(PTI6,0)//偏置常低

/************************************************/

void  LEDPIN_Init(void);   //LED控制引脚初始化

void  OLED_Init(void);

 void LED_CLS(void);
 
 void LED_Set_Pos(byte x, byte y);//设置坐标函数
 void LED_WrDat(U8 data);   //写数据函数
 
 void LED_P6x8Char(byte x,byte y,byte ch);
 void LED_P6x8Str(byte x,byte y,byte ch[]);
 void LED_P8x16Char(byte x,byte y,byte ch);
 void LED_P8x16Str(byte x,byte y,byte ch[]);
 

 
 void LED_PrintBMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]);
 
 void LED_Fill(byte dat);
 
 void LED_PrintValueC(U8 x, U8 y,char data);
 void LED_PrintValueI(U8 x, U8 y, int data);
 void LED_PrintValueF(U8 x, U8 y, float data, U8 num);

 void LED_Cursor(U8 cursor_column, U8 cursor_row);
 
 extern char logo[];


#endif
