#ifndef _GT9XX_H_
#define _GT9XX_H_

#include "my_iic.h"
#include "lcd.h"
#include "delay.h"
#include "stdio.h"

/* 端口定义 */
#define GT_RES_GPIO_PIN GPIO_Pin_0
#define GT_RES_GPIO_PORT GPIOE
#define GT_RES_GPIO_CLK    RCC_APB2Periph_GPIOE

#define GT_INT_GPIO_PIN GPIO_Pin_2
#define GT_INT_GPIO_PORT GPIOF
#define GT_INT_GPIO_CLK    RCC_APB2Periph_GPIOF

/* 端口中断定义 */
#define GT_INT_EXTI_PORT                 GPIO_PortSourceGPIOF
#define GT_INT_EXTI_PINSOURCE            GPIO_PinSource2
#define GT_INT_EXTI_LINE                 EXTI_Line2

#define GT_INT_EXTI_IRQ                  EXTI2_IRQn 
#define GT_IRQHandler                    EXTI2_IRQHandler
/* 端口电平状态 */
#define GT_RES_Clr() GPIO_ResetBits(GT_RES_GPIO_PORT,GT_RES_GPIO_PIN)
#define GT_RES_Set() GPIO_SetBits(GT_RES_GPIO_PORT,GT_RES_GPIO_PIN)

#define GT_INT_Clr() GPIO_ResetBits(GT_INT_GPIO_PORT,GT_INT_GPIO_PIN)
#define GT_INT_Set() GPIO_SetBits(GT_INT_GPIO_PORT,GT_INT_GPIO_PIN)

/* 触摸IC相关寄存器 */
#define ADDR_SET 0
#if ADDR_SET == 0
#define GT_Write_Addr 0xBA
#define GT_Read_Addr 0xBB
#else
#define GT_Write_Addr 0x28
#define GT_Read_Addr 0x29
#endif

#define GT_ID_Addr 0x8140
#define GT_CFG_Addr 0x8047
#define GT_Point_Addr 0x814E

/* 定义最多触摸点个数 */
#define TOUCH_MAX 5

/* 定义触摸状态 */
#define TP_PRES_DOWN 0x80
#define TP_CATH_PRES 0x40

typedef struct
{
    uint8_t flag;
    uint8_t num;
    uint8_t sta;
    uint16_t x[TOUCH_MAX];
    uint16_t y[TOUCH_MAX];
} TouchStructure;

extern TouchStructure TouchInfo;

/* 函数说明 */
void gt9xx_GPIOInit(void);
void gt9xx_Config(void);
void gt9xx_INT_In(void);
void gt9xx_INT_Out(void);
uint8_t gt9xx_WR_DATA(uint16_t addr, uint8_t data);
uint8_t gt9xx_RD_DATA(uint16_t addr, uint8_t len, uint8_t *value);
void gt9xx_Init(void);
void EXTI_enable(void);
void gt9xx_IntScan(void);
void gt9xx_Scan(uint8_t mode);
void gt9xx_Test(void);
void GT_Scan(uint8_t mode);
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);
#endif
