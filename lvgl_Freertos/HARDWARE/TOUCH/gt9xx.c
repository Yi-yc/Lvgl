#include "gt9xx.h"


/* 定义触摸控制结构体 */
TouchStructure TouchInfo = {
    0,
    0,
    0,
    0,
    0,
};

/* 定义触摸点地址 */
const uint16_t TPX[] = {0x8150, 0x8158, 0x8160, 0x8168, 0x8170};
/* 定义触摸点的颜色 */
const uint16_t POINT_COLOR_TBL[TOUCH_MAX] = {RED, GREEN, BLUE, GRAY, BLACK};

void gt9xx_GPIOInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(GT_RES_GPIO_CLK | GT_INT_GPIO_CLK, ENABLE); // 开启管脚对应时钟
   /********************配置GPIO***********************/

   GPIO_InitStructure.GPIO_Pin = GT_RES_GPIO_PIN;    // 配置RES管脚
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 配置为推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置IO口翻转速度为50Mhz
   GPIO_Init(GT_RES_GPIO_PORT, &GPIO_InitStructure); // 初始化RES管脚

   GPIO_InitStructure.GPIO_Pin = GT_INT_GPIO_PIN;    // 配置INT管脚
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 配置为推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置IO口翻转速度为50Mhz
   GPIO_Init(GT_INT_GPIO_PORT, &GPIO_InitStructure); // 初始化INT管脚
}

void gt9xx_INT_In(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(GT_INT_GPIO_CLK, ENABLE); // 开启管脚对应时钟
   /********************配置GPIO***********************/

   GPIO_InitStructure.GPIO_Pin = GT_INT_GPIO_PIN;    // 配置INT管脚
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // 配置为上拉输入
   GPIO_Init(GT_INT_GPIO_PORT, &GPIO_InitStructure); // 初始化INT管脚
}

void gt9xx_Config(void)
{
    gt9xx_GPIOInit();
    GT_RES_Clr();
    GT_INT_Clr();
    delay_ms(15);
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
    GT_RES_Set();
    delay_ms(15);
    /* 转为浮空输入 */
    gt9xx_INT_In();
}

uint8_t gt9xx_WR_DATA(uint16_t addr, uint8_t data)
{
    uint8_t reg;
    IIC_Start();
    IIC_SendByte(GT_Write_Addr);
    IIC_WaitAck();
    IIC_SendByte((uint8_t)(addr >> 8));
    IIC_WaitAck();
    IIC_SendByte((uint8_t)addr);
    IIC_WaitAck();
    IIC_SendByte(data);
    IIC_WaitAck();
    IIC_Stop();
    reg = 1;
    return reg;
}

uint8_t gt9xx_RD_DATA(uint16_t addr, uint8_t len, uint8_t *value)
{
    uint8_t reg = 0, i;
    IIC_Start();
    IIC_SendByte(GT_Write_Addr); // 发送写指令
    IIC_WaitAck();
    IIC_SendByte((uint8_t)(addr >> 8));
    IIC_WaitAck();
    IIC_SendByte((uint8_t)addr);
    IIC_WaitAck();
    IIC_Start();
    IIC_SendByte(GT_Read_Addr);
    IIC_WaitAck();
    for (i = 0; i < len; i++)
    {
        if (i == (len - 1))
        {
            value[i] = IIC_ReceiveByte(0x00);
        }
        else
        {
            value[i] = IIC_ReceiveByte(0x01);
        }
    }
    IIC_Stop();
    reg = 1;
    return reg;
}

void gt9xx_Init(void)
{
    uint8_t gt9xx_Info[11];
    uint8_t gt9xx_Version = 0;
    IIC_GPIOInit();
//    gt9xx_Config();
    gt9xx_RD_DATA(GT_ID_Addr, 11, gt9xx_Info);
    gt9xx_RD_DATA(GT_CFG_Addr, 1, &gt9xx_Version);
    if (gt9xx_Info[0] == '9') // 判断第一个字符是否为 9
    {
        printf("Touch ID: GT%.4s \r\n", gt9xx_Info);                                                                   // 打印触摸芯片的ID
        printf("固件版本： 0X%.4x\r\n", (gt9xx_Info[5] << 8) + gt9xx_Info[4]);                                         // 芯片固件版本
        printf("触摸分辨率：%d * %d\r\n", (gt9xx_Info[7] << 8) + gt9xx_Info[6], (gt9xx_Info[9] << 8) + gt9xx_Info[8]); // 当前触摸分辨率
        printf("触摸参数配置版本： 0X%.2x \r\n", gt9xx_Version);                                                       // 触摸配置版本
    }
    else
    {
        printf("Touch Error\r\n"); // 错误，未检测到触摸屏
    }
}

/**
 @brief     INT管脚中断使能
 @param     无
 @retval    无
*/
void EXTI_enable(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 外部中断需要使能复用时钟
   RCC_APB2PeriphClockCmd(GT_INT_GPIO_CLK, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GT_INT_GPIO_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GT_INT_GPIO_PORT, &GPIO_InitStructure);

   // 配置NVIC优先级分组
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   // 配置NVIC中断源
   NVIC_InitStructure.NVIC_IRQChannel = GT_INT_EXTI_IRQ;
   // 配置NVIC抢占优先级
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   // 配置NVIC子优先级
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
   // 使能中断通道
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   GPIO_EXTILineConfig(GT_INT_EXTI_PORT, GT_INT_EXTI_PINSOURCE);
   // 选择中断源
   EXTI_InitStructure.EXTI_Line = GT_INT_EXTI_LINE;
   // 选择中断模式
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   // 下降沿触发
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
   // 使能中断
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;

   EXTI_Init(&EXTI_InitStructure);
}

/**
 @brief     触摸中断扫描函数
 @param     无
 @retval    无
*/
void gt9xx_IntScan(void)
{
    uint8_t touchData[2 + 8 * TOUCH_MAX]; // 用于存储触摸数据
    uint8_t i = 0;
    gt9xx_RD_DATA(GT_Point_Addr, 2 + 8 * TOUCH_MAX, touchData); // 读数据
    gt9xx_WR_DATA(GT_Point_Addr, 0);                            // 清除触摸芯片的寄存器标志位
    TouchInfo.num = touchData[0] & 0x0f;                 // 取当前的触摸点数

    if ((TouchInfo.num >= 1) && (TouchInfo.num <= 5)) //  当触摸数在 1-5 之间时
    {
        for (i = 0; i < TouchInfo.num; i++) // 取相应的触摸坐标
        {
            if (USE_HORIZONTAL == 0)
            {
                TouchInfo.x[i] = ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // 获取X坐标
                TouchInfo.y[i] = ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]);         // 获取Y坐标
            }
            else if (USE_HORIZONTAL == 1)
            {
                TouchInfo.x[i] = LCD_W - ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // 获取X坐标
                TouchInfo.y[i] = LCD_H - ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]); // 获取Y坐标
            }
            else if (USE_HORIZONTAL == 2)
            {
                TouchInfo.x[i] = ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]);           // 获取X坐标
                TouchInfo.y[i] = LCD_H - ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // 获取Y坐标
            }
            else
            {
                TouchInfo.x[i] = LCD_W - ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]); // 获取X坐标
                TouchInfo.y[i] = ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // 获取Y坐标
            }
            printf("x[%d]:%d,y[%d]:%d\r\n", i, TouchInfo.x[i], i, TouchInfo.y[i]);
        }
        TouchInfo.flag = 1; // 触摸标志位置1，代表有触摸动作发生
    }
    else
    {
        TouchInfo.flag = 0; // 触摸标志位置0，无触摸动作
    }
}

/**
 @brief     触摸中断处理函数
 @param     无
 @retval    无
*/
void GT_IRQHandler(void)
{
   if (EXTI_GetITStatus(GT_INT_EXTI_LINE) != RESET)
   {
      gt9xx_IntScan();
      EXTI_ClearITPendingBit(GT_INT_EXTI_LINE);
   }
}

void GT_Scan(uint8_t mode)
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t temp;
    uint8_t tempsta;
    static uint8_t t = 0; // 控制查询间隔,从而降低CPU占用率
    t++;
    if ((t % 10) == 0 || t < 10) // 空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
    {
        gt9xx_RD_DATA(GT_Point_Addr, 1, &mode);
        if (mode & 0X80 && ((mode & 0XF) < 6))
        {
            temp = 0;
            gt9xx_WR_DATA(GT_Point_Addr, temp);
        }
        if ((mode & 0XF) && ((mode & 0XF) < 6))
        {
            temp = 0XFF << (mode & 0XF); // 将点的个数转换为1的位数,匹配TouchInfo.sta定义
            tempsta = TouchInfo.sta;     // 保存当前的TouchInfo.sta值
            TouchInfo.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            TouchInfo.x[4] = TouchInfo.x[0]; // 保存触点0的数据
            TouchInfo.y[4] = TouchInfo.y[0];
            for (i = 0; i < 5; i++)
            {
                if (TouchInfo.sta & (1 << i)) // 触摸有效?
                {
                    gt9xx_RD_DATA(TPX[i], 4, buf); // 读取XY坐标值
                    if (USE_HORIZONTAL == 0)
                    {

                        TouchInfo.x[i] = (((uint16_t)buf[1] << 8) + buf[0]);
                        TouchInfo.y[i] = (((uint16_t)buf[3] << 8) + buf[2]);
                    }
                    else if (USE_HORIZONTAL == 1)
                    {
                        TouchInfo.x[i] =LCD_W - (((uint16_t)buf[1] << 8) + buf[0]);
                        TouchInfo.y[i] =LCD_H - (((uint16_t)buf[3] << 8) + buf[2]);
                    }
                    else if (USE_HORIZONTAL == 2)
                    {
                        TouchInfo.x[i] = (((uint16_t)buf[3] << 8) + buf[2]);
                        TouchInfo.y[i] = LCD_H - (((uint16_t)buf[1] << 8) + buf[0]);
                    }
                    else
                    {
                        TouchInfo.x[i] =LCD_W - (((uint16_t)buf[3] << 8) + buf[2]);
                        TouchInfo.y[i] = (((uint16_t)buf[1] << 8) + buf[0]);
                    }
                    printf("x[%d]:%d,y[%d]:%d\r\n", i, TouchInfo.x[i], i, TouchInfo.y[i]);
                }
            }
            if (TouchInfo.x[0] > LCD_W || TouchInfo.y[0] > LCD_H) // 非法数据(坐标超出了)
            {
                if ((mode & 0XF) > 1) // 有其他点有数据,则复第二个触点的数据到第一个触点.
                {
                    TouchInfo.x[0] = TouchInfo.x[1];
                    TouchInfo.y[0] = TouchInfo.y[1];
                    t = 0; // 触发一次,则会最少连续监测10次,从而提高命中率
                }
                else // 非法数据,则忽略此次数据(还原原来的)
                {
                    TouchInfo.x[0] = TouchInfo.x[4];
                    TouchInfo.y[0] = TouchInfo.y[4];
                    mode = 0X80;
                    TouchInfo.sta = tempsta; // 恢复TouchInfo.sta
                }
            }
            else
                t = 0; // 触发一次,则会最少连续监测10次,从而提高命中率
        }
    }
    if ((mode & 0X8F) == 0X80) // 无触摸点按下
    {
        if (TouchInfo.sta & TP_PRES_DOWN) // 之前是被按下的
        {
            TouchInfo.sta &= ~(1 << 7); // 标记按键松开
        }
        else // 之前就没有被按下
        {
            TouchInfo.x[0] = 0xffff;
            TouchInfo.y[0] = 0xffff;
            TouchInfo.sta &= 0XE0; // 清除点有效标记
        }
    }
    if (t > 240)
        t = 10; // 重新从10开始计数
}

/**
 @brief     触摸画大点函数
 @param     无
 @retval    无
*/
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color)
{
   LCD_DrawPoint(x, y, color); // 中心点
   LCD_DrawPoint(x + 1, y, color);
   LCD_DrawPoint(x, y + 1, color);
   LCD_DrawPoint(x + 1, y + 1, color);
}

 /**
 * @brief   清空屏幕并在右上角显示"RST"
 * @param   无
 * @retval  无
 */
static void load_draw_dialog(void)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); /* 清屏 */
    LCD_ShowString(LCD_W-36,0,"RST",RED,WHITE,24,0);/* 显示清屏区域 */
}

/**
 * @brief   画粗线
 * @param   x1: 起点X坐标
 * @param   y1: 起点Y坐标
 * @param   x2: 终点X坐标
 * @param   y2: 终点Y坐标
 * @param   size: 线条粗细程度
 * @param   color: 线的颜色
 * @retval  无
 */
static void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0;
    int yerr = 0;
    int delta_x;
    int delta_y;
    int distance;
    int incx, incy, row, col;
    
    if ((x1 < size) || (x2 < size) || (y1 < size) || (y2 < size))
    {
        return;
    }
    
    delta_x = x2 - x1;                          /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;
    if (delta_x > 0)
    {
        incx = 1;                               /* 设置单步方向 */
    }
    else if (delta_x == 0)
    {
        incx = 0;                               /* 垂直线 */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    
    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0;                               /* 水平线 */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
    if (delta_x > delta_y)
    {
        distance = delta_x;                     /* 选取基本增量坐标轴 */
    }
    else
    {
        distance = delta_y;
    }
    
    for (t=0; t<=(distance + 1); t++)           /* 画线输出 */
    {
        LCD_DrawCircle(row,col,size,color,1);/* 画点 */
        xerr += delta_x;
        yerr += delta_y;
        
        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }
        
        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}


/**
 * @brief   电容触摸屏测试
 * @param   无
 * @retval  无
 */
void  gt9xx_Test(void)
{
    uint8_t t = 0;
    uint8_t i = 0;
    uint16_t lastpos[5][2];
    uint8_t maxp = 5;
    load_draw_dialog();
    while (1)
    {
        GT_Scan(0);
        for (t=0; t<maxp; t++)
        {
            if ((TouchInfo.sta) & (1 << t))
            {
                if ((TouchInfo.x[t] < LCD_W) && (TouchInfo.y[t] < LCD_H))                                  /* 坐标在屏幕范围内 */
                {
                    if (lastpos[t][0] == 0xFFFF)
                    {
                        lastpos[t][0] = TouchInfo.x[t];
                        lastpos[t][1] = TouchInfo.y[t];
                    }
                    
                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], TouchInfo.x[t], TouchInfo.y[t], 1, POINT_COLOR_TBL[t]);  /* 画线 */
                    lastpos[t][0] = TouchInfo.x[t];
                    lastpos[t][1] = TouchInfo.y[t];
                    
                    if (TouchInfo.x[t] > (LCD_W - 36) && TouchInfo.y[t] < 20)
                    {
                        load_draw_dialog();                                                                         /* 清除 */
                    }
                }
            }
            else
            {
                lastpos[t][0] = 0xFFFF;
            }
        }
        
        delay_ms(5);
        i++;
    }
} 


