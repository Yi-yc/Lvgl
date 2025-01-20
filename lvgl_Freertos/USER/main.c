#include "main.h"

void Hardware_Init()
{
	delay_init();	    	 														//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	Print_Init(115200);	 														//串口初始化为115200
	LED_Init();
	TIM3_Int_Init(9,7199);													//10Khz的计数频率，计数到5000为500ms  
	SPI1_Init();
	LCD_Init();
	printf("start\r\n");
}

static void switch_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) 
		{
				LED1=!LED1;
        printf("The object was clicked!\n");
    } 
		else if (event == LV_EVENT_PRESSED) 
		{
        printf("The object was PRESSED!\n");
    }
 
}

static void button_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) 
		{
				LED1=!LED1;
        printf("button was clicked!\n");
    } 
		else if (event == LV_EVENT_PRESSED) 
		{
        printf("button was PRESSED!\n");
    }
 
}


static void slider_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) 
		{
			  int value = lv_slider_get_value(obj);
        printf("Slider value: %d\r\n", value);
    } 
}


static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_VALUE_CHANGED) {
        char buf[32];
        lv_roller_get_selected_str(obj, buf, sizeof(buf));
        printf("Selected month: %s\n", buf);
    }
}

int main(void)
{		
	
		Hardware_Init();
		LCD_Fill(0,0,40,LCD_H,RED);
		LCD_Fill(40,0,80,LCD_H,GREEN);
		LCD_Fill(80,0,120,LCD_H,BLUE);
		LCD_Fill(120,0,160,LCD_H,YELLOW);
		LCD_Fill(180,0,200,LCD_H,WHITE);
		LCD_Fill(200,0,240,LCD_H,LIGHTBLUE);
	
#if LV_TICK_CUSTOM == 0
	
		lv_init();
		lv_port_disp_init();
		lv_port_indev_init();	
		
		/*Create a slider*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(slider, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 50);
		lv_obj_set_size(slider, 200, 15);  // 设置滑块的宽度为200，高度为20
		lv_slider_set_range(slider, 0, 100);
    lv_obj_set_event_cb(slider, slider_event_cb);

		lv_obj_t *roller1 = lv_roller_create(lv_scr_act(), NULL);
    lv_roller_set_options(roller1,
                        "January\n"
                        "February\n"
                        "March\n"
                        "April\n"
                        "May\n"
                        "June\n"
                        "July\n"
                        "August\n"
                        "September\n"
                        "October\n"
                        "November\n"
                        "December",
                        LV_ROLLER_MODE_INFINITE);

    lv_roller_set_visible_row_count(roller1, 4);
    lv_obj_align(roller1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(roller1, event_handler);	
	

		lv_obj_t *button;        
		button = lv_switch_create(lv_scr_act(), NULL);                                       
    lv_obj_set_size(button,100, 100);      
    lv_obj_align(button,NULL, LV_ALIGN_IN_TOP_LEFT, 200, 300);  
    lv_obj_set_event_cb(button, button_event_cb);
		while(1)
		{
			
				delay_ms(1);		
				lv_task_handler();
		}	 
		
#else

		lvgl_demo();
				
#endif
	
//		

 
}	 

