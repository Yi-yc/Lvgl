#include "lcd_init.h"

/**
 * @brief       端口初始化配置
 * @param       无
 * @retval      无
 */
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LCD_RES_GPIO_CLK|LCD_DC_GPIO_CLK|LCD_CS_GPIO_CLK,ENABLE);
    SPI1_Init();
    GPIO_InitStructure.GPIO_Pin=LCD_RES_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LCD_RES_GPIO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin=LCD_DC_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LCD_DC_GPIO_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=LCD_CS_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LCD_CS_GPIO_PORT,&GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin=LCD_BLK_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LCD_BLK_GPIO_PORT,&GPIO_InitStructure);
}

/**
 * @brief       IO模拟SPI发送一个字节数据
 * @param       dat: 需要发送的字节数据
 * @retval      无
 */
void LCD_WR_Bus(uint8_t dat)
{
    LCD_CS_Clr();
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//当发送缓冲器为NULL标志位被置1开始发送数据
    SPI_I2S_SendData(SPI1,dat);//SPI外设发送数据
    while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)==SET);//检测发送完成SPI外设处于空闲状态
    LCD_CS_Set();
}

/**
 * @brief       向液晶写寄存器命令
 * @param       reg: 要写的命令
 * @retval      无
 */
void LCD_WR_REG(uint8_t reg)
{
    LCD_DC_Clr();
    LCD_WR_Bus(reg);
    LCD_DC_Set();
}

/**
 * @brief       向液晶写一个字节数据
 * @param       dat: 要写的数据
 * @retval      无
 */
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_DC_Set();
    LCD_WR_Bus(dat);
    LCD_DC_Set();
}

/**
 * @brief       向液晶写一个半字数据
 * @param       dat: 要写的数据
 * @retval      无
 */
void LCD_WR_DATA(uint16_t dat)
{
    LCD_DC_Set();
    LCD_WR_Bus(dat >> 8);
    LCD_WR_Bus(dat & 0xFF);
    LCD_DC_Set();
}


/**
 * @brief       设置光标位置
 * @param       x:坐标列地址
 * @param       y:坐标行地址
 * @retval      无
 */
void LCD_SetCursor(uint16_t x, uint16_t y)
{
    LCD_Address_Set(x, y, x, y);
}




/**
 * @brief       设置显示窗口
 * @param       xs:窗口列起始地址
 * @param       ys:坐标行起始地址
 * @param       xe:窗口列结束地址
 * @param       ye:坐标行结束地址
 * @retval      无
 */
void LCD_Address_Set(uint16_t xs, uint16_t ys, uint16_t xe, uint16_t ye)
{
    LCD_WR_REG(0x2a); // 列地址设置
    LCD_WR_DATA(xs);
    LCD_WR_DATA(xe);
    LCD_WR_REG(0x2b); // 行地址设置
    LCD_WR_DATA(ys);
    LCD_WR_DATA(ye);
    LCD_WR_REG(0x2c); // 储存器写
}


void LCD_Init(void)
{
		LCD_GPIOInit();
    /* 触摸液晶一起复位 */
    LCD_RES_Set();
    delay_ms(50);
    LCD_RES_Clr();
    GT_INT_Clr();
    delay_ms(50);
    if (ADDR_SET == 1)
    {
        GT_INT_Set();
        delay_ms(1);
    }
    else
    {
        GT_INT_Clr();
        delay_ms(1);
    }
    LCD_RES_Set();
    delay_ms(120);
    gt9xx_INT_In();
    /* 获取触摸芯片信息 */
    gt9xx_Init();
    LCD_BLK_Set();

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0f);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x1B);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x3c);
    LCD_WR_DATA8(0x78);
    LCD_WR_DATA8(0x4A);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x1B);
    LCD_WR_DATA8(0x1e);
    LCD_WR_DATA8(0x0f);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x24);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x12);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x47);
    LCD_WR_DATA8(0x47);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x0a);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x30);
    LCD_WR_DATA8(0x37);
    LCD_WR_DATA8(0x0f);

    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x10);

    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0x41);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x80);

    LCD_WR_REG(0x36); /* Memory Access Control */
    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_DATA8(0x48);
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_DATA8(0x88);
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_DATA8(0x28);
    }
    else
    {
        LCD_WR_DATA8(0xE8);
    }

    LCD_WR_REG(0x3A); /* Interface Mode Control */
    LCD_WR_DATA8(0x55);

    LCD_WR_REG(0XB0);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xB1); /* Frame rate 70HZ */
    LCD_WR_DATA8(0xB0);
    LCD_WR_DATA8(0x11);
    LCD_WR_REG(0xB4);
    LCD_WR_DATA8(0x02);
    LCD_WR_REG(0xB6); /* RGB-MCU Interface Control */
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x02);

    LCD_WR_REG(0xB7);
    LCD_WR_DATA8(0xC6);
    LCD_WR_REG(0xE9);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0XF7);
    LCD_WR_DATA8(0xA9);
    LCD_WR_DATA8(0x51);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x82);

    LCD_WR_REG(0x11);
    delay_ms(120);
    LCD_WR_REG(0x29);
} 



