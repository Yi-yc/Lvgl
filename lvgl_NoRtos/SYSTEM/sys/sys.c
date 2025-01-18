#include "sys.h"

//********************************************************************************  
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}




void Hardware_Init()
{
	delay_init();	    	 														//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	Print_Init(115200);	 														//���ڳ�ʼ��Ϊ115200
	Usart3_Init(115200);														//����3������ESP8266��
	LED_Init();
	TIM3_Int_Init(9,7199);													//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	SPI1_Init();
	LCD_Init();
	printf("start\r\n");
}