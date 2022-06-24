#ifndef _BSP_SPI_LCD_H_
#define _BSP_SPI_LCD_H_

#include "main.h"
#include "bsp_spi_lcd_font.h"
#include "bsp_spi_lcd_picture.h"

/*
STM32_HAL SPI_LCD XuanLib
2022/06/21
changelog：
1.0：基于SPI驱动的LCD驱动
*/

//引脚定义(需要修改Init函数开启对应GPIO时钟，并配置SPI)
#define LCD_BLK_PORT GPIOA
#define LCD_BLK_PIN  GPIO_PIN_2
#define LCD_DC_PORT  GPIOA
#define LCD_DC_PIN   GPIO_PIN_3
#define LCD_CS_PORT  GPIOA
#define LCD_CS_PIN   GPIO_PIN_4

//LCD功能函数
void LCD_Init(void);

//引脚操作函数

void LCD_PIN_Write(uint8_t LCD_PIN, GPIO_PinState Status);

#endif
