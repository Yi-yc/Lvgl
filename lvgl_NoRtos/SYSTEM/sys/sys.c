#include "sys.h"

//********************************************************************************  
//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//关闭所有中断
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//开启所有中断
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}




void Hardware_Init()
{
	delay_init();	    	 														//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Print_Init(115200);	 														//串口初始化为115200
	Usart3_Init(115200);														//串口3，驱动ESP8266用
	LED_Init();
	TIM3_Int_Init(9,7199);													//10Khz的计数频率，计数到5000为500ms  
	SPI1_Init();
	LCD_Init();
	printf("start\r\n");
}