#ifndef _BSP_F411_LED_H_
#define _BSP_F411_LED_H_

#include "main.h"
#include "stdbool.h"

/*
STM32_HAL F411_LED XuanLib V1.0
2022/06/16
changelog：
1.0：实现F411开发板板载数码管与LED灯控制
*/

/*
默认引脚配置
74HC138：
A0 -> PC10
A1 -> PC11
A2 -> PC12
74HC595：
DATA -> PB3
LCLK -> PB4
SCLK -> PB5
*/

//使用RTC功能
#define RTC_DISABLE
//使用FreeRTOS功能
#define FreeRTOS

//74HC138芯片引脚(共阴极数码管段选端，低电平有效，默认上拉)
#define HC138_A0_PORT HC138_A0_GPIO_Port
#define HC138_A1_PORT HC138_A1_GPIO_Port
#define HC138_A2_PORT HC138_A2_GPIO_Port
#define HC138_A0_PIN HC138_A0_Pin
#define HC138_A1_PIN HC138_A1_Pin
#define HC138_A2_PIN HC138_A2_Pin
//74HC595芯片引脚(共阴极数码管位选端，高电平有效，芯片上升沿有效，默认下拉)
#define HC595_DATA_PORT HC595_DATA_GPIO_Port  //数据输入端
#define HC595_LCLK_PORT HC595_LCLK_GPIO_Port  //数据清零端
#define HC595_SCLK_PORT HC595_SCLK_GPIO_Port  //数据位移端
#define HC595_DATA_PIN HC595_DATA_Pin
#define HC595_LCLK_PIN HC595_LCLK_Pin
#define HC595_SCLK_PIN HC595_SCLK_Pin
//用于F411_SEG函数不使用小数点
#define Point_Position_None 0

//引脚操作函数

void HC138_A0(GPIO_PinState Status);
void HC138_A1(GPIO_PinState Status);
void HC138_A2(GPIO_PinState Status);
void HC595_DATA(GPIO_PinState Status);
void HC595_LCLK(GPIO_PinState Status);
void HC595_SCLK(GPIO_PinState Status);

//芯片操作函数

void HC138(uint8_t Binary);
void HC595(uint8_t NumberCode[]);

//功能函数

void F411_LED(long LED_Status);  //控制全部LED灯
void F411_LED_Single(uint8_t LED_Position, GPIO_PinState LED_Status);  //控制单个LED灯
void F411_SEG(long SEG_Show, uint8_t Point_Positon);  //控制全部数码管
void F411_SEG_Single(uint8_t SEG_Position, uint8_t SEG_Show, bool Point);  //控制单个数码管

#ifdef RTC_ENABLE
#include "bsp_rtc.h"
void F411_SEG_RTC(void);
#endif

#ifdef FreeRTOS
#include "cmsis_os.h"
#define YMX_Delay osDelay
#else  //不使用FreeRTOS
#include "bsp_delay.h"
#define YMX_Delay Delay_ms
#endif

#endif
