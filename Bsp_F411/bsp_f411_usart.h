#ifndef _BSP_F411_USART_H_
#define _BSP_F411_USART_H_

#include "usart.h"
#include "stdio.h"

/*
STM32_HAL F411_USART XuanLib V1.0
2022/06/17
changelog：
1.0：实现F411板载DAP-Link串口使用，注意中文编码
*/

//USART最大接收字节数
#define USART_RX_LENGTH      200
#define USART_RX_BUFFER_SIZE 1

extern uint8_t USART_RX_BUFFER[USART_RX_LENGTH];
extern uint16_t USART_RX_STATUS;
extern uint8_t aRXBuffer[USART_RX_BUFFER_SIZE];

void F411_USART_Init(void);  //初始化USART1

#endif
