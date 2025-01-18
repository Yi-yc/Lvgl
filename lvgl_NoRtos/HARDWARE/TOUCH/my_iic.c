#include "my_iic.h"
#include "delay.h"

/**
 @brief     初始化IIC外设GPIO
 @param     无
 @retval    无
*/
void IIC_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(IIC_SCL_GPIO_CLK | IIC_SDA_GPIO_CLK, ENABLE); // 开启管脚对应时钟
    /********************配置GPIO***********************/
    
    GPIO_InitStructure.GPIO_Pin = IIC_SCL_GPIO_PIN;    // 配置SCL管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    // 配置为开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置IO口翻转速度为50Mhz
    GPIO_Init(IIC_SCL_GPIO_PORT, &GPIO_InitStructure); // 初始化SCL管脚

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;    // 配置SDA管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    // 配置为开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置IO口翻转速度为50Mhz
    GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure); // 初始化SDA管脚
}

/**
 @brief     配置IIC SDA信号线为输入模式
 @param     无
 @retval    无
*/
void IIC_SDAIN()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(IIC_SDA_GPIO_CLK, ENABLE); // 开启管脚对应时钟
    /********************配置GPIO***********************/

    GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;    // 配置SDA管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;    // 配置为上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置IO口翻转速度为50Mhz
    GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure); // 初始化SDA管脚
}

/**
 @brief     配置IIC SDA信号线为输出模式
 @param     无
 @retval    无
*/
void IIC_SDAOUT()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(IIC_SDA_GPIO_CLK, ENABLE); // 开启管脚对应时钟
    /********************配置GPIO***********************/
    
    GPIO_InitStructure.GPIO_Pin = IIC_SDA_GPIO_PIN;    // 配置SDA管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;    // 配置为开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置IO口翻转速度为50Mhz
    GPIO_Init(IIC_SDA_GPIO_PORT, &GPIO_InitStructure); // 初始化SDA管脚
}

/**
 @brief     IIC通信内部延时
 @param     无
 @retval    无
*/
void IIC_delay(void)
{
    __nop();__nop();
}

/**
 @brief     IIC起始信号
 @param     无
 @retval    无
*/
void IIC_Start(void)
{
    IIC_SDAOUT();
    IIC_SCL_Set();
    IIC_SDA_Set();
    IIC_delay();
    IIC_SDA_Clr(); /* 起始信号 */
    IIC_delay();
    IIC_SCL_Clr(); /* 钳住IIC总线准备收发数据 */
    IIC_delay();
}

/**
 @brief     IIC结束信号
 @param     无
 @retval    无
*/
void IIC_Stop(void)
{
    IIC_SDAOUT();
    IIC_SDA_Clr();
    IIC_SCL_Set();
    IIC_delay();
    IIC_SDA_Set(); /* 结束信号 */
    IIC_delay();
}

/**
 @brief     IIC应答信号
 @param     无
 @retval    无
*/
void IIC_Ack(void)
{
    IIC_SDAOUT();
    IIC_SDA_Clr(); /* 应答信号 */
    IIC_delay();
    IIC_SCL_Set(); /* 产生一个时钟 */
    IIC_delay();
    IIC_SCL_Clr();
    IIC_delay();
    IIC_SDA_Set(); /* 释放SDA控制权 */
    IIC_delay();
}

/**
 @brief     IIC非应答信号
 @param     无
 @retval    无
*/
void IIC_Nack(void)
{
    IIC_SDAOUT();
    IIC_SDA_Set(); /* 非应答信号 */
    IIC_delay();
    IIC_SCL_Set(); /* 产生一个时钟 */
    IIC_delay();
    IIC_SCL_Clr();
    IIC_delay();
}

/**
 @brief     IIC等待应答函数
 @param     无
 @retval    1:接收应答失败
            0:接收应答成功
*/
uint8_t IIC_WaitAck(void)
{
    uint8_t waittime = 0;
    uint8_t receive_status=0;
    IIC_SDAIN();
    IIC_SDA_Set(); /* 主机释放SDA线 */
    IIC_delay();
    IIC_SCL_Set(); /* 主机等待响应信号 */
    IIC_delay();
    
    while (IIC_ReadSDA)
    {
        waittime++;
        if (waittime >= 250)
        {
            IIC_Stop();
            receive_status = 1;
            break;
        }
    }
    IIC_SCL_Clr(); /* 等待结束结束ACK检查 */
    IIC_delay();
    return receive_status;
}

/**
 @brief     IIC发送一个字节数据
 @param     dat：需要发送的数据
 @retval    无
*/
void IIC_SendByte(uint8_t dat)
{
    uint8_t i;
    IIC_SDAOUT();
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            IIC_SDA_Set();
        }
        else
        {
            IIC_SDA_Clr();
        }
        IIC_delay();
        IIC_SCL_Set(); /* SCL高电平 SDA的数据有效 */
        IIC_delay();
        IIC_SCL_Clr();
        dat <<= 1;
    }
    IIC_SDA_Set(); /* 发送完成释放SDA总线 */
}

/**
 @brief     IIC读取一个字节数据
 @param     ack：0发送非应答 1发送应答
 @retval    接收到的响应信号
*/
uint8_t IIC_ReceiveByte(uint8_t ack)
{
    uint8_t i, receive=0;
    IIC_SDAIN();
    for (i = 0; i < 8; i++)
    {
        receive <<= 1; /* 先发高位所以数据左移 */
        IIC_SCL_Set();
        IIC_delay();
        if (IIC_ReadSDA)
        {
            receive++;
        }
        IIC_SCL_Clr();
        IIC_delay();
    }
    if (!ack)
    {
        IIC_Nack();
    }
    else
    {
        IIC_Ack();
    }
    return receive;
}
