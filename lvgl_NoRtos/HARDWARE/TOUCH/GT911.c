#include "GT911.h"
#include "delay.h"
#include "lcd_init.h"
const u16 POINT_COLOR_TBL[CT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED};  
//���ݴ��������Ժ���

void Load_Drow_Dialog(void)
{
	LCD_ShowString(LCD_W-24,0,"RST",RED,WHITE,16,0);//��ʾ��������
}
//////////////////////////////////////////////////////////////////////////////////
//��һ������
//(x1,y1),(x2,y2):��������ʼ����
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		TP_Draw_Big_Point(uRow,uCol,color);//����
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   

void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	    
 	u16 lastpos[5][2];		//���һ�ε����� 
	while(1)
	{   
		tp_dev.scan(0);

		for(t=0;t<CT_MAX_TOUCH;t++)//���5�㴥��
		{
			if((tp_dev.sta)&(1<<t))//�ж��Ƿ��е㴥����
			{    
				delay_ms(1);
				if(tp_dev.x[t]<LCD_W&&tp_dev.y[t]<LCD_H)//��LCD��Χ��
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],POINT_COLOR_TBL[t]);//����
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(LCD_W-24)&&tp_dev.y[t]<16)
					{
						
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		delay_ms(5);i++;
	}	
}


