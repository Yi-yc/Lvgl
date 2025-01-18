#include "dma.h"


u8 SPI1_TX_BUFFER[BUFFER_DATA];

void DMA_Configuration(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

	/* DMA1 Channel3 (triggered by SPI1 Tx event) Config */
	DMA_DeInit(DMA1_Channel3);  
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;                           
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI1_TX_BUFFER;                    
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                               
	DMA_InitStructure.DMA_BufferSize = 32400;                         
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;                 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                           
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;          
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;                  
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                                     
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                           
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                      
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);        
	/* Enable SPI1 DMA TX request */
	SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);                                                         
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
}
