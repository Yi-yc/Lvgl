/**
 ****************************************************************************************************
 * @file        lvgl_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-11
 * @brief       LVGL V8����ϵͳ��ֲ
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
 
#include "lvgl_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"


/******************************************************************************************************/
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */

/* LV_DEMO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LV_DEMO_TASK_PRIO   3           /* �������ȼ� */
#define LV_DEMO_STK_SIZE    1024        /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;       /* ������ */
void lv_demo_task(void *pvParameters);  /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       4           /* �������ȼ� */
#define LED_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t LEDTask_Handler;           /* ������ */
void led_task(void *pvParameters);      /* ������ */
/******************************************************************************************************/

/**
 * @brief       lvgl_demo��ں���
 * @param       ��
 * @retval      ��
 */
void lvgl_demo(void)
{
    lv_init();                                          /* lvglϵͳ��ʼ�� */
    lv_port_disp_init();                                /* lvgl��ʾ�ӿڳ�ʼ��,����lv_init()�ĺ��� */
    lv_port_indev_init();                               /* lvgl����ӿڳ�ʼ��,����lv_init()�ĺ��� */

    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */

    vTaskStartScheduler();                              /* ����������� */
}

/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����LVGL���� */
    xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED�������� */
    xTaskCreate((TaskFunction_t )led_task,
                (const char*    )"led_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&LEDTask_Handler);

    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
}


// 
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

static void slider_event_cb(lv_obj_t * obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) 
		{
			  int value = lv_slider_get_value(obj);
        printf("Slider value: %d\r\n", value);
    } 
		
 
}


/**
 * @brief       LVGL��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
    

    /*Create a slider*/
    lv_obj_t * slider = lv_slider_create(lv_scr_act(), NULL);
    lv_obj_align(slider, NULL, LV_ALIGN_IN_TOP_LEFT, 50, 300);
		lv_obj_set_size(slider, 200, 20);  // ���û���Ŀ��Ϊ200���߶�Ϊ20
		lv_slider_set_range(slider, 0, 100);
    lv_obj_set_event_cb(slider, slider_event_cb);
		
		lv_obj_t *switch_cool;        
		switch_cool = lv_switch_create(lv_scr_act(), NULL);                                       
    lv_obj_set_size(switch_cool,100, 100);      
    lv_obj_align(switch_cool,NULL, LV_ALIGN_IN_TOP_LEFT, 0, 150);  
		lv_obj_set_event_cb(switch_cool, switch_event_cb);
	
    while(1)
    {
        lv_task_handler(); /* LVGL��ʱ�� */
        vTaskDelay(5);
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
void led_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {
				printf("led\r\n");
        vTaskDelay(1000);
    }
}


