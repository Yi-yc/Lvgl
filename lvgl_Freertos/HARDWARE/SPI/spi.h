#ifndef __SPI_H_
#define __SPI_H_



#include "main.h"


/* º¯ÊýÉùÃ÷ */
void SPI1_Init(void);
void SPI1_WriteByte(u8 dat);
void LCD_GPIOInit(void);
void LCD_WR_Bus(uint8_t dat);
void LCD_WR_REG(uint8_t reg);
void LCD_WR_DATA8(uint8_t dat);
void LCD_WR_DATA(uint16_t dat);





#endif 