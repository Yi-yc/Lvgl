#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "main.h"


void Print_Init(u32 bound);
void Usart3_Init(u32 bound);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);

#endif


