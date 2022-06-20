#ifndef _BSP_AT24CXX_H_
#define _BSP_AT24CXX_H_

#include "main.h"
#include "stdbool.h"
#include "bsp_i2c.h"

/*
STM32_HAL AT24CXX XuanLib
2022/06/20
changelog：
1.0：实现AT24CXX的读写
1.1：重写以适配新I2C库
*/

//AT24CXX ID
#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767

//STM32F411开发板板载AT24C02
extern uint16_t EEPROM_TYPE;

void AT24CXX_Init(uint16_t AT24CXX);  //初始化
bool AT24CXX_Check(void);  //检查器件

uint8_t AT24CXX_ReadByte(uint16_t Addr);  //指定地址读取一个字节
uint32_t AT24CXX_ReadByte_Length(uint16_t Addr, uint8_t Length);  //指定地址读取指定长度数据
void AT24CXX_Read(uint16_t Addr, uint8_t* pBuffer, uint16_t NumToRead);  //从指定地址开始读取指定长度数据

void AT24CXX_WriteByte(uint16_t Addr, uint8_t Data);  //指定地址写入一个字节
void AT24CXX_WriteByte_Length(uint16_t Addr, uint32_t Data, uint8_t Length);  //指定地址写入指定长度数据
void AT24CXX_Write(uint16_t Addr, uint8_t* pBuffer, uint16_t NumToWrite);  //从指定地址开始写入指定长度数据

#endif
