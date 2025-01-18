#ifndef _CTIIC_H_
#define _CTIIC_H_
#include "sys.h"
#include "delay.h"	 
#include "touch.h"
#include "lcd_init.h"
#define TOUCH_MAX 5
 

#define IIC_PIN_SCL		GPIO_Pin_0
#define IIC_PIN_SDA		GPIO_Pin_1
#define IIC_PIN_INT		GPIO_Pin_2


//IO方向设置
#define CT_SDA_IN()  {GPIOF->CRL&=0XFFFFFF0F;GPIOF->CRL|=0x00000080;}
#define CT_SDA_OUT() {GPIOF->CRL&=0XFFFFFF0F;GPIOF->CRL|=0x00000030;}

#define RST_OUT()		//{GPIOE->CRL&=0XFFFFFFF0;GPIOE->CRL|=0X00000003;}		//set RSSET pin to output


#define INT_OUT()		{GPIOF->CRL&=0XFFFFF0FF;GPIOF->CRL|=0X00000300;}		//set RSSET pin to output
#define INT_IN()		{GPIOF->CRL&=0XFFFFF0FF;GPIOF->CRL|=0X00000400;}		//set RSSET pin to input
	


//IO操作函数	 
#define CT_IIC_SCL    PFout(0) 			//SCL   

#define CT_IIC_SDA    PFout(1) 			//SDA	 
#define CT_READ_SDA   PFin(1)  			//输入SDA 

#define RST_CTRL   		PGout(0)	//GT911 RESET pin out high or low
#define INT_CTRL   		PFout(2) 	//GT911 INT pin out high or low
#define INT_GET   		PFin(2) 	//Get GT911 INT pin status


#define GT911_IIC_RADDR 0xBB			//IIC read address, should be 0x29
#define GT911_IIC_WADDR 0xBA			//IIC write address, should be 0x28

#define GT911_READ_ADDR 0x814E		//touch point information
#define GT911_ID_ADDR 	0x8140		//ID of touch IC



void CT_IIC_Init(void);                	//初始化IIC的IO口				 
void CT_IIC_Start(void);								//发送IIC开始信号
void CT_IIC_Stop(void);	  							//发送IIC停止信号
void CT_IIC_Send_Byte(u8 txd);					//IIC发送一个字节
u8 CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
u8 CT_IIC_Wait_Ack(void); 							//IIC等待ACK信号
void CT_IIC_Ack(void);									//IIC发送ACK信号
void CT_IIC_NAck(void);									//IIC不发送ACK信号

void GT911_int_sync(u16 ms);
void GT911_reset_guitar(u8 addr);
void GT911_gpio_init(void);
u8 GT911_WriteHandle (u16 addr);
u8 GT911_WriteData (u16 addr,u8 value);
u8 GT911_ReadData (u16 addr, u8 cnt, u8 *value);
u8 GT911_Init(void);
u8 Touch_Get_Count(void);
u8 GT911_Scan(u8 mode);
void GT911_send_config(void);
void GT911_Eint_Init(void);

#endif

