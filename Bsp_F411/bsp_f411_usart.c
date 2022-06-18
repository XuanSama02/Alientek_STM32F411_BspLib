#include "bsp_f411_usart.h"

#pragma import(__use_no_semihosting)

//标准库需要的支持函数
struct __FILE
{
    int handle;
};
FILE __stdout;

//定义_sys_exit()以免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}

//重定义fptuc函数
int fputc(int ch, FILE *f)
{
  while((USART1->SR&0x40) == 0);  //循环发送
    USART1->DR = (uint8_t)ch;
  return ch;
}

uint8_t USART_RX_BUFFER[USART_RX_LENGTH];  //接收缓存
uint16_t USART_RX_STATUS;                  //接收状态
uint8_t aRXBuffer[USART_RX_BUFFER_SIZE];   //HAL使用的串口接收缓存

void F411_USART_Init(void)
{
  HAL_UART_Receive_IT(&huart1, (uint8_t*)aRXBuffer, USART_RX_BUFFER_SIZE);
}
