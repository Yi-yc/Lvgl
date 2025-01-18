#include "CTIIC.h"


//控制I2C速度的延时
void CT_Delay(void)
{
	delay_us(10);
} 
//电容触摸芯片IIC接口初始化
//void CT_IIC_Init(void)
//{					     
// 	RCC->APB2ENR|=1<<4;		//先使能外设IO PORTC时钟    
//	GPIOC->CRL&=0XFFFF0FF0;	//PC0,PC3 推挽输出
//	GPIOC->CRL|=0X00003003;	   
//	GPIOC->ODR|=1<<1;	    //PC0 输出高	 
//	GPIOC->ODR|=1<<3;	    //PC3 输出高	 
//}
//产生IIC起始信号
void CT_IIC_Start(void)
{
	CT_SDA_OUT();     //sda线输出
	CT_IIC_SDA=1;	  	  
	CT_IIC_SCL=1;
	CT_Delay();
 	CT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	CT_Delay();
	CT_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void CT_IIC_Stop(void)
{ 
	CT_SDA_OUT();//sda线输出
	CT_IIC_SCL=0;
	CT_IIC_SDA=0;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SDA=1;//STOP:when CLK is high DATA change form low to high 
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 CT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	CT_SDA_IN();      //SDA设置为输入  
	CT_IIC_SDA=1;delay_us(1);	   
	CT_IIC_SCL=1;delay_us(1);	 
	while(CT_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			CT_IIC_Stop();
			return 1;
		} 
	}
	CT_IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void CT_IIC_Ack(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_IIC_SDA=0;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SCL=0;
}
//不产生ACK应答		    
void CT_IIC_NAck(void)
{
	CT_IIC_SCL=0;
	CT_SDA_OUT();
	CT_IIC_SDA=1;
	CT_Delay();
	CT_IIC_SCL=1;
	CT_Delay();
	CT_IIC_SCL=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void CT_IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	CT_SDA_OUT(); 	    
    CT_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        CT_IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	      
		CT_IIC_SCL=1;
		CT_Delay();
		CT_IIC_SCL=0;	
		CT_Delay();
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 CT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
 	CT_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        CT_IIC_SCL=0; 	    	   
		delay_us(30);
		CT_IIC_SCL=1;  
		receive<<=1;
		if(CT_READ_SDA)receive++;   
	}	  				 
	if (!ack)CT_IIC_NAck();//发送nACK
	else CT_IIC_Ack(); //发送ACK   
 	return receive;
}

//==================================================================
//Name:						GT911_int_sync
//Author:					
//Date:						2017-07-06
//Function:				Sync GT911 INT signal
//								Sync is the most important part in initial stage
//Input:					unsigned int
//Output:					void
//option:
//==================================================================
void GT911_int_sync(u16 ms)
{
	INT_CTRL=0;
	delay_ms(ms);
	INT_IN();
}	

//==================================================================
//Name:						GT911_reset_guitar
//Author:					
//Date:						2017-07-06
//Function:				Rest GT911, and work with function GT911_int_sync
//								sync sequence depends on slave address
//Input:					unsigned char addr
//Output:					void
//option:
//==================================================================
void GT911_reset_guitar(u8 addr)
{
	INT_CTRL=1;
	RST_CTRL=1;
	delay_ms(20);
//	RST_CTRL=0;
	INT_CTRL=0;
	delay_ms(20);
	INT_CTRL=(addr == 0x28);
	delay_ms(20);
	RST_CTRL=1;
	delay_ms(20);
}

//==================================================================
//Name:						GT911_reset
//Author:					
//Date:						2017-07-06
//Function:				Rest GT911, just reset without sync
//Input:					void
//Output:					void
//option:
//==================================================================
void GT911_reset(void)
{
//	RST_CTRL=0;
	delay_ms(10);
	RST_CTRL=1;
	delay_ms(10);
}

//==================================================================
//Name:						GT911_gpio_init
//Author:					
//Date:						2017-07-06
//Function:				Init GT911 related GPIO: IIC,RST,INT
//Input:					void
//Output:					void
//option:
//==================================================================
void GT911_gpio_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);//使能PC端口时钟

	GPIO_InitStructure.GPIO_Pin =IIC_PIN_SCL|IIC_PIN_SDA|IIC_PIN_INT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF,IIC_PIN_SCL|IIC_PIN_SDA|IIC_PIN_INT);									


	RST_OUT();
	INT_OUT();
	GT911_reset();
}

//==================================================================
//Name:						GT9XX_WriteHandle
//Author:					
//Date:						2017-07-06
//Function:				Handle GT911 write function
//Input:					unsigned int addr
//Output:					void
//option:
//==================================================================
u8 GT911_WriteHandle (u16 addr)
{
	u8 status;

	CT_IIC_Start();
	CT_IIC_Send_Byte(GT911_IIC_WADDR); //写数据指令
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte((u8)(addr >> 8)); //写入16位地址
	CT_IIC_Wait_Ack();
	CT_IIC_Send_Byte((u8)addr);
	CT_IIC_Wait_Ack();
	status = SUCCESS;
	return status;	
}

//==================================================================
//Name:						GT9XX_WriteData
//Author:					
//Date:						2017-07-06
//Function:				Write data to GT911
//Input:					unsigned int addr				//address of register
//								unsigned char value			//value should be writed
//Output:					void
//option:
//==================================================================
u8 GT911_WriteData (u16 addr,u8 value)
{
	u8 status;
	CT_IIC_Start();

	GT911_WriteHandle(addr);
	CT_IIC_Send_Byte(value);
	CT_IIC_Wait_Ack();
	CT_IIC_Stop();	
	status = SUCCESS;
	return status;
}

//==================================================================
//Name:						GT9XX_ReadData
//Author:					
//Date:						2017-07-06
//Function:				Read data from GT911
//Input:					unsigned int addr				//address of register
//								unsigned char *value		//pointer of data output
//Output:					void
//option:
//==================================================================
u8 GT911_ReadData (u16 addr, u8 cnt, u8 *value)
{
	u8 status;
	u8 i;

	status = ERROR;
	CT_IIC_Start();
	GT911_WriteHandle(addr);
	CT_IIC_Start();
	delay_us(20);
	CT_IIC_Send_Byte(GT911_IIC_RADDR);
	CT_IIC_Wait_Ack();
	for(i = 0 ; i < cnt; i++)
	{
		if (i == (cnt - 1))
		{
			value[i] = CT_IIC_Read_Byte(0);
		}
		else
		{
			value[i] = CT_IIC_Read_Byte(1);
		}
	}					
	CT_IIC_Stop();
	status = SUCCESS;
	return (status);	
}


//==================================================================
//Name:						Touch_Init
//Author:					
//Date:						2017-07-06
//Function:				GT911 init, including GPIO init, sync, and version check
//Input:					unsigned int addr				//address of register
//								unsigned char *value		//pointer of data output
//Output:					void
//option:
//==================================================================
u8 GT911_Init(void)
{
	u8 touchIC_ID[4];	
	GT911_gpio_init();
	GT911_reset_guitar(GT911_IIC_WADDR);
	GT911_int_sync(50);
	GT911_ReadData (GT911_ID_ADDR,4,touchIC_ID);
	if( touchIC_ID[0] == '9' )
	{
		printf("Touch ID: %s \r\n",touchIC_ID);
		return 1;
	}
	else
	{
		printf("Touch Error\r\n");
		return 0;
	}
}

u8 Touch_Get_Count(void)
{
	u8 count[1] = {0};
	GT911_ReadData (GT911_READ_ADDR,1,count);	//read touch data
	return (count[0]&0x0f);
}

const u16 TPX[] = {0x8150,0x8158,0x8160,0x8168,0x8170}; //电容屏触摸点数据地址（1~5）

//==================================================================
//Name:						Touch_Get_Data
//Author:					
//Date:						2017-07-06
//Function:				Get GT911 data, such as point and coordinate
//Input:					void
//Output:					void
//option:
//==================================================================
u8 GT911_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
 	static u8 t=0;//控制查询间隔,从而降低CPU占用率   
	t++;
	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{ 
 		GT911_ReadData(GT911_READ_ADDR, 1, &mode);
		if(mode&0X80&&((mode&0XF)<6))
		{
			temp=0;	
			GT911_WriteData (GT911_READ_ADDR,temp);
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//将点的个数转换为1的位数,匹配tp_dev.sta定义 
			tempsta=tp_dev.sta;			//保存当前的tp_dev.sta值
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//保存触点0的数据
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//触摸有效?
				{
					GT911_ReadData(TPX[i],4,buf);	//读取XY坐标值
          if(USE_HORIZONTAL==2)//横屏
					{
						tp_dev.y[i]=320-((u16)buf[1]<<8)-buf[0];
						tp_dev.x[i]=(((u16)buf[3]<<8)+buf[2]);
					}
					else if(USE_HORIZONTAL==0)
					{
						tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
					} 
					else if(USE_HORIZONTAL==1)
					{
						tp_dev.x[i]=320-(((u16)buf[1]<<8)+buf[0]);
						tp_dev.y[i]=480-(((u16)buf[3]<<8)+buf[2]);
					} 
					else 
					{
						tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
						tp_dev.x[i]=480-(((u16)buf[3]<<8)+buf[2]);
					} 					
					
				}			
			} 
			printf("x[%d]:%d,y[%d]:%d\r\n",0,tp_dev.x[0],0,tp_dev.y[0]);
			res=1;
			if(tp_dev.x[0]>LCD_W||tp_dev.y[0]>LCD_H)//非法数据(坐标超出了)
			{ 
				if((mode&0XF)>1)		//有其他点有数据,则复第二个触点的数据到第一个触点.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//触发一次,则会最少连续监测10次,从而提高命中率
				}else					//非法数据,则忽略此次数据(还原原来的)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//恢复tp_dev.sta
				}
			}else t=0;					//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}
	if((mode&0X8F)==0X80)//无触摸点按下
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
		{
			tp_dev.sta&=~(1<<7);	//标记按键松开
		}else						//之前就没有被按下
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//清除点有效标记	
		}	 
	} 	
	if(t>240)t=10;//重新从10开始计数
	return res;
}



