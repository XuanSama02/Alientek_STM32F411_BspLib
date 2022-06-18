#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

#include "main.h"
#include "stdbool.h"
#include "bsp_delay_tim.h"

/*
STM32_HAL I2C XuanLib V1.0
2022/06/17
changelog：
1.0：实现软件I2C接口(PB7=SDA，PB6=SCL)
*/

/*
I2C_SCL = PB6
I2C_SDA = PB7
*/

//I2C模式
#define IN  (bool)0
#define OUT (bool)1

//I2C_SDA模式切换函数

void SDA_Mode(bool Mode);

//I2C_IO操作函数

void I2C_SCL_Write(GPIO_PinState Status);
void I2C_SDA_Write(GPIO_PinState Status);
GPIO_PinState I2C_SCL_Read(void);
GPIO_PinState I2C_SDA_Read(void);


//I2C_操作函数

void I2C_Start(void);  //发送I2C开始信号
void I2C_Stop(void);   //发送I2C停止信号
void I2C_Send_Byte(uint8_t TxData);        //I2C发送一个字节
uint8_t I2C_Read_Byte(unsigned char Ack);  //I2C读取一个字节
bool I2C_Wait_Ack(void);  //I2C等待Ack信号

void I2C_Ack(void);   //I2C发送Ack信号
void I2C_NAck(void);  //I2C不发送Ack信号

#endif
