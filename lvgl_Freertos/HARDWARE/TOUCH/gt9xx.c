#include "gt9xx.h"


/* ���崥�����ƽṹ�� */
TouchStructure TouchInfo = {
    0,
    0,
    0,
    0,
    0,
};

/* ���崥�����ַ */
const uint16_t TPX[] = {0x8150, 0x8158, 0x8160, 0x8168, 0x8170};
/* ���崥�������ɫ */
const uint16_t POINT_COLOR_TBL[TOUCH_MAX] = {RED, GREEN, BLUE, GRAY, BLACK};

void gt9xx_GPIOInit(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(GT_RES_GPIO_CLK | GT_INT_GPIO_CLK, ENABLE); // �����ܽŶ�Ӧʱ��
   /********************����GPIO***********************/

   GPIO_InitStructure.GPIO_Pin = GT_RES_GPIO_PIN;    // ����RES�ܽ�
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // ����Ϊ�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ����IO�ڷ�ת�ٶ�Ϊ50Mhz
   GPIO_Init(GT_RES_GPIO_PORT, &GPIO_InitStructure); // ��ʼ��RES�ܽ�

   GPIO_InitStructure.GPIO_Pin = GT_INT_GPIO_PIN;    // ����INT�ܽ�
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // ����Ϊ�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // ����IO�ڷ�ת�ٶ�Ϊ50Mhz
   GPIO_Init(GT_INT_GPIO_PORT, &GPIO_InitStructure); // ��ʼ��INT�ܽ�
}

void gt9xx_INT_In(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_APB2PeriphClockCmd(GT_INT_GPIO_CLK, ENABLE); // �����ܽŶ�Ӧʱ��
   /********************����GPIO***********************/

   GPIO_InitStructure.GPIO_Pin = GT_INT_GPIO_PIN;    // ����INT�ܽ�
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;     // ����Ϊ��������
   GPIO_Init(GT_INT_GPIO_PORT, &GPIO_InitStructure); // ��ʼ��INT�ܽ�
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
    /* תΪ�������� */
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
    IIC_SendByte(GT_Write_Addr); // ����дָ��
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
    if (gt9xx_Info[0] == '9') // �жϵ�һ���ַ��Ƿ�Ϊ 9
    {
        printf("Touch ID: GT%.4s \r\n", gt9xx_Info);                                                                   // ��ӡ����оƬ��ID
        printf("�̼��汾�� 0X%.4x\r\n", (gt9xx_Info[5] << 8) + gt9xx_Info[4]);                                         // оƬ�̼��汾
        printf("�����ֱ��ʣ�%d * %d\r\n", (gt9xx_Info[7] << 8) + gt9xx_Info[6], (gt9xx_Info[9] << 8) + gt9xx_Info[8]); // ��ǰ�����ֱ���
        printf("�����������ð汾�� 0X%.2x \r\n", gt9xx_Version);                                                       // �������ð汾
    }
    else
    {
        printf("Touch Error\r\n"); // ����δ��⵽������
    }
}

/**
 @brief     INT�ܽ��ж�ʹ��
 @param     ��
 @retval    ��
*/
void EXTI_enable(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   EXTI_InitTypeDef EXTI_InitStructure;
   NVIC_InitTypeDef NVIC_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // �ⲿ�ж���Ҫʹ�ܸ���ʱ��
   RCC_APB2PeriphClockCmd(GT_INT_GPIO_CLK, ENABLE);
   GPIO_InitStructure.GPIO_Pin = GT_INT_GPIO_PIN;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GT_INT_GPIO_PORT, &GPIO_InitStructure);

   // ����NVIC���ȼ�����
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   // ����NVIC�ж�Դ
   NVIC_InitStructure.NVIC_IRQChannel = GT_INT_EXTI_IRQ;
   // ����NVIC��ռ���ȼ�
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
   // ����NVIC�����ȼ�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
   // ʹ���ж�ͨ��
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);

   GPIO_EXTILineConfig(GT_INT_EXTI_PORT, GT_INT_EXTI_PINSOURCE);
   // ѡ���ж�Դ
   EXTI_InitStructure.EXTI_Line = GT_INT_EXTI_LINE;
   // ѡ���ж�ģʽ
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   // �½��ش���
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
   // ʹ���ж�
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;

   EXTI_Init(&EXTI_InitStructure);
}

/**
 @brief     �����ж�ɨ�躯��
 @param     ��
 @retval    ��
*/
void gt9xx_IntScan(void)
{
    uint8_t touchData[2 + 8 * TOUCH_MAX]; // ���ڴ洢��������
    uint8_t i = 0;
    gt9xx_RD_DATA(GT_Point_Addr, 2 + 8 * TOUCH_MAX, touchData); // ������
    gt9xx_WR_DATA(GT_Point_Addr, 0);                            // �������оƬ�ļĴ�����־λ
    TouchInfo.num = touchData[0] & 0x0f;                 // ȡ��ǰ�Ĵ�������

    if ((TouchInfo.num >= 1) && (TouchInfo.num <= 5)) //  ���������� 1-5 ֮��ʱ
    {
        for (i = 0; i < TouchInfo.num; i++) // ȡ��Ӧ�Ĵ�������
        {
            if (USE_HORIZONTAL == 0)
            {
                TouchInfo.x[i] = ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // ��ȡX����
                TouchInfo.y[i] = ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]);         // ��ȡY����
            }
            else if (USE_HORIZONTAL == 1)
            {
                TouchInfo.x[i] = LCD_W - ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // ��ȡX����
                TouchInfo.y[i] = LCD_H - ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]); // ��ȡY����
            }
            else if (USE_HORIZONTAL == 2)
            {
                TouchInfo.x[i] = ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]);           // ��ȡX����
                TouchInfo.y[i] = LCD_H - ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // ��ȡY����
            }
            else
            {
                TouchInfo.x[i] = LCD_W - ((touchData[5 + 8 * i] << 8) | touchData[4 + 8 * i]); // ��ȡX����
                TouchInfo.y[i] = ((touchData[3 + 8 * i] << 8) | touchData[2 + 8 * i]); // ��ȡY����
            }
            printf("x[%d]:%d,y[%d]:%d\r\n", i, TouchInfo.x[i], i, TouchInfo.y[i]);
        }
        TouchInfo.flag = 1; // ������־λ��1�������д�����������
    }
    else
    {
        TouchInfo.flag = 0; // ������־λ��0���޴�������
    }
}

/**
 @brief     �����жϴ�����
 @param     ��
 @retval    ��
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
    static uint8_t t = 0; // ���Ʋ�ѯ���,�Ӷ�����CPUռ����
    t++;
    if ((t % 10) == 0 || t < 10) // ����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
    {
        gt9xx_RD_DATA(GT_Point_Addr, 1, &mode);
        if (mode & 0X80 && ((mode & 0XF) < 6))
        {
            temp = 0;
            gt9xx_WR_DATA(GT_Point_Addr, temp);
        }
        if ((mode & 0XF) && ((mode & 0XF) < 6))
        {
            temp = 0XFF << (mode & 0XF); // ����ĸ���ת��Ϊ1��λ��,ƥ��TouchInfo.sta����
            tempsta = TouchInfo.sta;     // ���浱ǰ��TouchInfo.staֵ
            TouchInfo.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;
            TouchInfo.x[4] = TouchInfo.x[0]; // ���津��0������
            TouchInfo.y[4] = TouchInfo.y[0];
            for (i = 0; i < 5; i++)
            {
                if (TouchInfo.sta & (1 << i)) // ������Ч?
                {
                    gt9xx_RD_DATA(TPX[i], 4, buf); // ��ȡXY����ֵ
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
            if (TouchInfo.x[0] > LCD_W || TouchInfo.y[0] > LCD_H) // �Ƿ�����(���곬����)
            {
                if ((mode & 0XF) > 1) // ��������������,�򸴵ڶ�����������ݵ���һ������.
                {
                    TouchInfo.x[0] = TouchInfo.x[1];
                    TouchInfo.y[0] = TouchInfo.y[1];
                    t = 0; // ����һ��,��������������10��,�Ӷ����������
                }
                else // �Ƿ�����,����Դ˴�����(��ԭԭ����)
                {
                    TouchInfo.x[0] = TouchInfo.x[4];
                    TouchInfo.y[0] = TouchInfo.y[4];
                    mode = 0X80;
                    TouchInfo.sta = tempsta; // �ָ�TouchInfo.sta
                }
            }
            else
                t = 0; // ����һ��,��������������10��,�Ӷ����������
        }
    }
    if ((mode & 0X8F) == 0X80) // �޴����㰴��
    {
        if (TouchInfo.sta & TP_PRES_DOWN) // ֮ǰ�Ǳ����µ�
        {
            TouchInfo.sta &= ~(1 << 7); // ��ǰ����ɿ�
        }
        else // ֮ǰ��û�б�����
        {
            TouchInfo.x[0] = 0xffff;
            TouchInfo.y[0] = 0xffff;
            TouchInfo.sta &= 0XE0; // �������Ч���
        }
    }
    if (t > 240)
        t = 10; // ���´�10��ʼ����
}

/**
 @brief     ��������㺯��
 @param     ��
 @retval    ��
*/
void TP_Draw_Big_Point(uint16_t x, uint16_t y, uint16_t color)
{
   LCD_DrawPoint(x, y, color); // ���ĵ�
   LCD_DrawPoint(x + 1, y, color);
   LCD_DrawPoint(x, y + 1, color);
   LCD_DrawPoint(x + 1, y + 1, color);
}

 /**
 * @brief   �����Ļ�������Ͻ���ʾ"RST"
 * @param   ��
 * @retval  ��
 */
static void load_draw_dialog(void)
{
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE); /* ���� */
    LCD_ShowString(LCD_W-36,0,"RST",RED,WHITE,24,0);/* ��ʾ�������� */
}

/**
 * @brief   ������
 * @param   x1: ���X����
 * @param   y1: ���Y����
 * @param   x2: �յ�X����
 * @param   y2: �յ�Y����
 * @param   size: ������ϸ�̶�
 * @param   color: �ߵ���ɫ
 * @retval  ��
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
    
    delta_x = x2 - x1;                          /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;
    if (delta_x > 0)
    {
        incx = 1;                               /* ���õ������� */
    }
    else if (delta_x == 0)
    {
        incx = 0;                               /* ��ֱ�� */
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
        incy = 0;                               /* ˮƽ�� */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    
    if (delta_x > delta_y)
    {
        distance = delta_x;                     /* ѡȡ�������������� */
    }
    else
    {
        distance = delta_y;
    }
    
    for (t=0; t<=(distance + 1); t++)           /* ������� */
    {
        LCD_DrawCircle(row,col,size,color,1);/* ���� */
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
 * @brief   ���ݴ���������
 * @param   ��
 * @retval  ��
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
                if ((TouchInfo.x[t] < LCD_W) && (TouchInfo.y[t] < LCD_H))                                  /* ��������Ļ��Χ�� */
                {
                    if (lastpos[t][0] == 0xFFFF)
                    {
                        lastpos[t][0] = TouchInfo.x[t];
                        lastpos[t][1] = TouchInfo.y[t];
                    }
                    
                    lcd_draw_bline(lastpos[t][0], lastpos[t][1], TouchInfo.x[t], TouchInfo.y[t], 1, POINT_COLOR_TBL[t]);  /* ���� */
                    lastpos[t][0] = TouchInfo.x[t];
                    lastpos[t][1] = TouchInfo.y[t];
                    
                    if (TouchInfo.x[t] > (LCD_W - 36) && TouchInfo.y[t] < 20)
                    {
                        load_draw_dialog();                                                                         /* ��� */
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


