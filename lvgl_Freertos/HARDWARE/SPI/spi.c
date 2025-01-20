#include "spi.h"

/**
 * @brief       SPI�ӿڳ�ʼ��
 * @param       ��
 * @retval      ��
 */
void SPI1_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);                                    // ����SPIʱ��
    RCC_APB2PeriphClockCmd(LCD_CS_GPIO_CLK | LCD_SCK_GPIO_CLK | LCD_MOSI_GPIO_CLK, ENABLE); // �����ܽŶ�Ӧʱ��
    /********************����GPIO***********************/

    GPIO_InitStructure.GPIO_Pin = LCD_SCK_GPIO_PIN;    // ����SCK�ܽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;    // ����Ϊ�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // ����IO�ڷ�ת�ٶ�Ϊ50Mhz
    GPIO_Init(LCD_SCK_GPIO_PORT, &GPIO_InitStructure); // ��ʼ��SCK�ܽ�
    GPIO_SetBits(LCD_SCK_GPIO_PORT, LCD_SCK_GPIO_PIN); // ����SCK�ܽ�

    GPIO_InitStructure.GPIO_Pin = LCD_MOSI_GPIO_PIN;     // ����MOSI�ܽ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      // ����Ϊ�����������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    // ����IO�ڷ�ת�ٶ�Ϊ50Mhz
    GPIO_Init(LCD_MOSI_GPIO_PORT, &GPIO_InitStructure);  // ��ʼ��MOSI�ܽ�
    GPIO_SetBits(LCD_MOSI_GPIO_PORT, LCD_MOSI_GPIO_PIN); // ����MOSI�ܽ�

    /********************����SPI����***********************/
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPI1����APB2(�����ٶ�:72Mhz)2��ƵΪ36Mhz
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                       // �ڶ���ʱ���ؽ��в���
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                        // ����ʱ��״̬Ϊ��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  // ���ݿ��Ϊ8bit
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // ȫ˫��ģʽ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 // �������ݸ�ʽ��λ��ǰ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      // SPI1��Ϊ����ģʽ����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // SPIƬѡͨ���������
    SPI_Init(SPI1, &SPI_InitStructure);                                // ����SPI1
    SPI_Cmd(SPI1, ENABLE);                                             // ʹ��SPI1
}

