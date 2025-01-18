#ifndef _MY_IIC_H_
#define _MY_IIC_H_

#include "stm32f10x.h"

/* 端口定义 */
#define IIC_SCL_GPIO_PORT   GPIOF
#define IIC_SCL_GPIO_PIN    GPIO_Pin_0
#define IIC_SCL_GPIO_CLK    RCC_APB2Periph_GPIOB

#define IIC_SDA_GPIO_PORT   GPIOF
#define IIC_SDA_GPIO_PIN    GPIO_Pin_1
#define IIC_SDA_GPIO_CLK    RCC_APB2Periph_GPIOB

/* 端口电平状态 */
#define IIC_SCL_Clr() GPIO_ResetBits(IIC_SCL_GPIO_PORT,IIC_SCL_GPIO_PIN)
#define IIC_SCL_Set() GPIO_SetBits(IIC_SCL_GPIO_PORT,IIC_SCL_GPIO_PIN)

#define IIC_SDA_Clr() GPIO_ResetBits(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN)
#define IIC_SDA_Set() GPIO_SetBits(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN)

#define IIC_ReadSDA   GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN)

/* 函数声明 */
void IIC_GPIOInit(void);
void IIC_delay(void);
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_Nack(void);
uint8_t IIC_WaitAck(void);
void IIC_SendByte(uint8_t dat);
uint8_t IIC_ReceiveByte(uint8_t ack);

#endif


