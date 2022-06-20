#ifndef _BSP_RTC_H_
#define _BSP_RTC_H_

#include "main.h"
#include "rtc.h"
#include "stdio.h"

/*
STM32_HAL RTC XuanLib
2022/06/15
changelog：
1.0：实现RTC相关功能
1.1：头文件添加stdio.h，完善注释
*/

extern RTC_TimeTypeDef YMX_Time;  //目前的时间
extern RTC_DateTypeDef YMX_Date;  //目前的日期
extern RTC_TimeTypeDef YMX_Time_Set;  //想要设置的时间
extern RTC_DateTypeDef YMX_Date_Set;  //想要设置的日期

void RTC_Init(void);

void RTC_SetTime(uint8_t Hours, uint8_t Minutes, uint8_t Seconds);  //设置新时间
void RTC_SetDate(uint8_t Year, uint8_t Month, uint8_t Date, uint8_t WeekDay);  //设置新日期
RTC_TimeTypeDef RTC_GetTime(void);  //获取目前的时间
RTC_DateTypeDef RTC_GetDate(void);  //获取目前的日期
void RTC_RefreshTime(void);  //刷新目前的时间与日期
void RTC_PrintTime(void);  //将目前的时间与日期通过串口打印

#endif
