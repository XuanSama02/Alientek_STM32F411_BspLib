#ifndef _BSP_F411_H_
#define _BSP_F411_H_

#include "main.h"
#include "stdbool.h"
#include "cmsis_os.h"

#include "bsp_f411_usart.h"
#include "bsp_f411_led.h"
#include "bsp_f411_beep.h"
#include "bsp_delay_tim.h"
#include "bsp_rtc.h"
#include "bsp_i2c.h"
#include "bsp_f411_tpad.h"
#include "bsp_w25qxx.h"
#include "bsp_at24cxx.h"
#include "bsp_oled.h"

/*
STM32_HAL F411 XuanLib
2022/06/19
changelog：
1.0：实现F411开发板板载资源测试
*/

//F411开发板Debug函数

void F411_Debug(void);

//F411开发板测试函数

void F411_Test(void);
void F411_Test_W25Q16(void);
void F411_Test_AT24C02(void);
void F411_Test_TPAD(void);

#endif
