#ifndef __LCD_INIT_H
#define __LCD_INIT_H



#include "main.h"

#define INTERFACE SPI_HARD



#define USE_HORIZONTAL 0 //���ú�������������ʾ 0��1Ϊ���� 2��3Ϊ����
#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 320
#define LCD_H 480

#else
#define LCD_W 480
#define LCD_H 320
#endif

#define BUFFER_DATA 	LCD_W*LCD_H*2

/* ����ܽŶ˿� */
#define LCD_SCK_GPIO_PORT   GPIOA
#define LCD_SCK_GPIO_PIN    GPIO_Pin_5
#define LCD_SCK_GPIO_CLK    RCC_APB2Periph_GPIOA

#define LCD_MOSI_GPIO_PORT   GPIOA
#define LCD_MOSI_GPIO_PIN    GPIO_Pin_7
#define LCD_MOSI_GPIO_CLK    RCC_APB2Periph_GPIOA

#define LCD_RES_GPIO_PORT   GPIOE
#define LCD_RES_GPIO_PIN    GPIO_Pin_0
#define LCD_RES_GPIO_CLK    RCC_APB2Periph_GPIOE

#define LCD_DC_GPIO_PORT    GPIOE
#define LCD_DC_GPIO_PIN     GPIO_Pin_1
#define LCD_DC_GPIO_CLK     RCC_APB2Periph_GPIOE

#define LCD_CS_GPIO_PORT    GPIOE
#define LCD_CS_GPIO_PIN     GPIO_Pin_2
#define LCD_CS_GPIO_CLK     RCC_APB2Periph_GPIOE

#define LCD_BLK_GPIO_PORT    GPIOE
#define LCD_BLK_GPIO_PIN     GPIO_Pin_6
#define LCD_BLK_GPIO_CLK     RCC_APB2Periph_GPIOE


//-----------------LCD�˿ڶ���---------------- 
#if(INTERFACE==SPI_HARD)


/* ����˿ڵ�ƽ״̬ */
#define LCD_SCK_Clr() GPIO_ResetBits(LCD_SCK_GPIO_PORT,LCD_SCK_GPIO_PIN)
#define LCD_SCK_Set() GPIO_SetBits(LCD_SCK_GPIO_PORT,LCD_SCK_GPIO_PIN)

#define LCD_MOSI_Clr() GPIO_ResetBits(LCD_MOSI_GPIO_PORT,LCD_MOSI_GPIO_PIN)
#define LCD_MOSI_Set() GPIO_SetBits(LCD_MOSI_GPIO_PORT,LCD_MOSI_GPIO_PIN)

#define LCD_RES_Clr() GPIO_ResetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)
#define LCD_RES_Set() GPIO_SetBits(LCD_RES_GPIO_PORT,LCD_RES_GPIO_PIN)

#define LCD_DC_Clr() GPIO_ResetBits(LCD_DC_GPIO_PORT,LCD_DC_GPIO_PIN)
#define LCD_DC_Set() GPIO_SetBits(LCD_DC_GPIO_PORT,LCD_DC_GPIO_PIN)

#define LCD_CS_Clr() GPIO_ResetBits(LCD_CS_GPIO_PORT,LCD_CS_GPIO_PIN)
#define LCD_CS_Set() GPIO_SetBits(LCD_CS_GPIO_PORT,LCD_CS_GPIO_PIN)

#define LCD_BLK_Clr() GPIO_ResetBits(LCD_BLK_GPIO_PORT,LCD_BLK_GPIO_PIN)
#define LCD_BLK_Set() GPIO_SetBits(LCD_BLK_GPIO_PORT,LCD_BLK_GPIO_PIN)


#else

#define LCD_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_0)//SCL=SCLK
#define LCD_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define LCD_MOSI_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_1)//SDA=MOSI
#define LCD_MOSI_Set() GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define LCD_RES_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//RES
#define LCD_RES_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define LCD_DC_Clr()   GPIO_ResetBits(GPIOA,GPIO_Pin_3)//DC
#define LCD_DC_Set()   GPIO_SetBits(GPIOA,GPIO_Pin_3)
 		     
#define LCD_CS_Clr()   GPIO_ResetBits(GPIOA,GPIO_Pin_4)//CS
#define LCD_CS_Set()   GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LCD_BLK_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_5)//BLK
#define LCD_BLK_Set()  GPIO_SetBits(GPIOA,GPIO_Pin_5)

#endif

void LCD_GPIO_Init(void);//��ʼ��GPIO
void LCD_Writ_Bus(u8 dat);//ģ��SPIʱ��
void LCD_WR_DATA8(u8 dat);//д��һ���ֽ�
void LCD_WR_DATA(u16 dat);//д�������ֽ�
void LCD_WR_REG(u8 dat);//д��һ��ָ��
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//�������꺯��
void LCD_Init(void);//LCD��ʼ��
#endif