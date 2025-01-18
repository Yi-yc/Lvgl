#include "main.h"

static void slider_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) 
		{
			  int value = lv_slider_get_value(obj);
        printf("Slider value: %d\r\n", value);
    } 
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


int main(void)
{		
	
	Hardware_Init();

//	ESP8266_Init();
	lv_init();
	lv_port_disp_init();
	lv_port_indev_init();
		
	lv_obj_t *switch_cool;        
	switch_cool = lv_switch_create(lv_scr_act(), NULL);                                       
	lv_obj_set_size(switch_cool,100, 100);      
	lv_obj_align(switch_cool,NULL, LV_ALIGN_IN_TOP_LEFT, 0, 150);  
	lv_obj_set_event_cb(switch_cool, switch_event_cb);
	


	lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
	lv_obj_align(slider, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 300);
	lv_obj_set_size(slider, 200, 20);  // 设置滑块的宽度为200，高度为20
	lv_slider_set_range(slider, 0, 100);
	lv_obj_set_event_cb(slider, slider_event_cb);

	
	while(1)
	{

		delay_ms(5);		
		lv_task_handler();
	}	 

 
}	 
 
