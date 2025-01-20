#include "spi.h"

/**
 * @brief       SPI接口初始化
 * @param       无
 * @retval      无
 */
void SPI1_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                                    // 开启SPI时钟
    RCC_APB2PeriphClockCmd(LCD_CS_GPIO_CLK | LCD_SCK_GPIO_CLK | LCD_MOSI_GPIO_CLK, ENABLE); // 开启管脚对应时钟
    /********************配置GPIO***********************/

    GPIO_InitStructure.GPIO_Pin = LCD_SCK_GPIO_PIN;    // 配置SCK管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // 配置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置IO口翻转速度为50Mhz
    GPIO_Init(LCD_SCK_GPIO_PORT, &GPIO_InitStructure); // 初始化SCK管脚
    GPIO_SetBits(LCD_SCK_GPIO_PORT, LCD_SCK_GPIO_PIN); // 拉高SCK管脚

    GPIO_InitStructure.GPIO_Pin = LCD_MOSI_GPIO_PIN;     // 配置MOSI管脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // 配置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // 配置IO口翻转速度为50Mhz
    GPIO_Init(LCD_MOSI_GPIO_PORT, &GPIO_InitStructure);  // 初始化MOSI管脚
    GPIO_SetBits(LCD_MOSI_GPIO_PORT, LCD_MOSI_GPIO_PIN); // 拉高MOSI管脚

    /********************配置SPI外设***********************/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPI1挂载APB2(总线速度:72Mhz)2分频为36Mhz
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                       // 第二个时钟沿进行采样
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                        // 空闲时钟状态为高
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  // 数据宽度为8bit
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // 全双工模式
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 // 发送数据格式高位在前
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      // SPI1作为主机模式工作
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // SPI片选通过软件控制
    SPI_Init(SPI1, &SPI_InitStructure);                                // 配置SPI1
    SPI_Cmd(SPI1, ENABLE);                                             // 使能SPI1
}

