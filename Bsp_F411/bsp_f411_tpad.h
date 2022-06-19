#ifndef _BSP_F411_TPAD_H_
#define _BSP_F411_TPAD_H_

#include "main.h"
#include "stdbool.h"
#include "cmsis_os.h"
#include "tim.h"

/*
STM32_HAL TouchPad XuanLib V1.0
2022/06/19
changelog：
1.0：未能实现板载TouchPad响应(不依赖CubeMX初始化代码)，初始化成功
*/

/*
使用定时器：
TIM3.CH4 = PB1
TIM3是16位定时器
*/
#define TPAD_GPIO_PORT GPIOB
#define TPAD_GPIO_PIN  GPIO_PIN_1

//16位定时器最大值
#define TPAD_ARR_MAX 0xFFFF
//TouchPad触摸阈值：[捕获值 > (TPAD_Default_Value+TPAD_Gate_Value)]，触发触摸
#define TPAD_Gate_Value 30

//TPAD按键悬空默认值：
extern uint16_t TPAD_Default_Value;

bool F411_TPAD_Init(uint16_t PSC);
bool F411_TPAD_Scan(bool Mode);
void F411_TPAD_Reset(void);

uint16_t F411_TPAD_Get_Value(void);
uint16_t F411_TPAD_Get_MaxValue(uint8_t Times);
void F411_TPAD_TIM3_Init(uint32_t ARR, uint16_t PSC);

#endif
