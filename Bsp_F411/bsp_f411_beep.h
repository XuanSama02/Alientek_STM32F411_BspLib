#ifndef _BSP_F411_BEEP_H_
#define _BSP_F411_BEEP_H_

#include "main.h"
#include "stdbool.h"

/*
STM32_HAL F411_BEEP XuanLib V1.0
2022/06/15
changelog：
1.0：实现对F411开发板板载蜂鸣器的控制
*/

//引脚配置
#define BEEP_PORT BEEP_GPIO_Port
#define BEEP_PIN BEEP_Pin
//状态定义
#define ON  (bool)1
#define OFF (bool)0

void F411_Beep(bool Status);

#endif
