#ifndef _BSP_OLED_H_
#define _BSP_OLED_H_

#include "main.h"
#include "stdbool.h"
#include "bsp_i2c.h"

/*
STM32_HAL I2C_OLED XuanLib
2022/06/20
changelog：
1.0：实现I2C协议控制SSD1306芯片驱动OLED屏幕基本显示功能(有BUG)
*/

//OLED默认设备地址
#define OLED_Addr_Dev 0x78

//OLED字体大小
#define TextSize_Small (bool)0
#define TextSize_Big   (bool)1

void OLED_Init(GPIO_TypeDef* SCL_PORT, uint16_t SCL_PIN, GPIO_TypeDef* SDA_PORT, uint16_t SDA_PIN);  //初始化OLED屏幕
void OLED_Command(uint8_t Data);  //I2C发送指令
void OLED_Data(uint8_t Data);  //I2C发送数据

void OLED_SetPos(uint8_t x, uint8_t y);  //设置坐标起始点
void OLED_Fill(uint8_t Fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t ch[], bool TextSize);

#endif
