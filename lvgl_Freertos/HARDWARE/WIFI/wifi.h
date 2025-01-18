#ifndef __WIFI_H
#define __WIFI_H

#include "main.h"


#define REV_OK		0	//接收完成标志
#define REV_WAIT	1	//接收未完成标志

extern unsigned char esp8266_buf[512];
extern unsigned short esp8266_cnt , esp8266_cntPre ;


void ESP8266_Init(void);
void ESP8266_Clear(void);
_Bool ESP8266_SendCmd(char *cmd, char *res);
void ESP8266_SendData(unsigned char *data, unsigned short len);
unsigned char *ESP8266_GetIPD(unsigned short timeOut);


#endif 