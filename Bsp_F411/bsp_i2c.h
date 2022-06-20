#ifndef _BSP_I2C_H_
#define _BSP_I2C_H_

#include "main.h"
#include "stdbool.h"
#include "bsp_delay_tim.h"

/*
STM32_HAL I2C XuanLib
2022/06/20
changelog：
1.0：实现软件I2C接口(PB7=SDA，PB6=SCL)
2.0：重构bsp库，使用类似HAL库的语法，实现任意两个GPIO口I2C协议通讯
*/

/**
 * @brief 存储I2C引脚配置信息
 * 
 */
typedef struct
{   
    GPIO_InitTypeDef I2C_GPIO_OUT;
    GPIO_InitTypeDef I2C_GPIO_IN;
}I2C_InitTypeDef;

/**
 * @brief 存储I2C配置信息，初始化只需要配置引脚的4个参数，不需要配置I2C_Mode和Init
 * 
 */
typedef struct
{
    bool I2C_Mode;  //SDA线模式
    GPIO_TypeDef* I2C_SCL_PORT;  //SCL端口
    GPIO_TypeDef* I2C_SDA_PORT;  //SDA端口
    uint16_t I2C_SCL_PIN;  //SCL引脚
    uint16_t I2C_SDA_PIN;  //SDA引脚
    I2C_InitTypeDef Init;  //I2C初始化配置
}I2C_HandleTypeDef;

//I2C引脚
#define I2C_PIN_SCL (bool)0
#define I2C_PIN_SDA (bool)1

//I2C模式
#define I2C_MODE_IN  (bool)0
#define I2C_MODE_OUT (bool)1

//I2C初始化函数

void I2C_Init(I2C_HandleTypeDef *hi2c);

//I2C_IO操作函数

void I2C_Mode(I2C_HandleTypeDef *hi2c, bool I2C_Mode);
void I2C_Write(I2C_HandleTypeDef *hi2c, bool I2C_PIN, GPIO_PinState Status);
GPIO_PinState I2C_Read(I2C_HandleTypeDef *hi2c, bool I2C_PIN);

//I2C_操作函数

void I2C_Start(I2C_HandleTypeDef *hi2c);  //发送I2C开始信号
void I2C_Stop(I2C_HandleTypeDef *hi2c);   //发送I2C停止信号
void I2C_Send_Byte(I2C_HandleTypeDef *hi2c, uint8_t TxData);        //I2C发送一个字节
uint8_t I2C_Read_Byte(I2C_HandleTypeDef *hi2c, unsigned char Ack);  //I2C读取一个字节
bool I2C_Wait_Ack(I2C_HandleTypeDef *hi2c);  //I2C等待Ack信号

void I2C_Ack(I2C_HandleTypeDef *hi2c);   //I2C发送Ack信号
void I2C_NAck(I2C_HandleTypeDef *hi2c);  //I2C不发送Ack信号

#endif
