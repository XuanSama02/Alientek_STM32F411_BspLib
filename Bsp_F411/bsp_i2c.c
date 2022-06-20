#include "bsp_i2c.h"

/**
 * @brief 初始化I2C接口，注意需要手动开启GPIO时钟
 * 
 */
void I2C_Init(I2C_HandleTypeDef *hi2c)
{
    hi2c->I2C_Mode = I2C_MODE_OUT;
    //输出模式引脚配置
    hi2c->Init.I2C_GPIO_OUT.Alternate = 0;  //不复用
    hi2c->Init.I2C_GPIO_OUT.Mode = GPIO_MODE_OUTPUT_PP;  //推挽输出
    hi2c->Init.I2C_GPIO_OUT.Pull = GPIO_PULLUP;  //上拉模式
    hi2c->Init.I2C_GPIO_OUT.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //输入模式引脚配置
    hi2c->Init.I2C_GPIO_IN.Alternate = 0;  //不复用
    hi2c->Init.I2C_GPIO_IN.Mode = GPIO_MODE_INPUT;  //输入模式
    hi2c->Init.I2C_GPIO_IN.Pull = GPIO_PULLUP;  //上拉模式
    hi2c->Init.I2C_GPIO_IN.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //初始化引脚(输出模式)
    hi2c->Init.I2C_GPIO_OUT.Pin = hi2c->I2C_SCL_PIN;
    HAL_GPIO_Init(hi2c->I2C_SCL_PORT, &(hi2c->Init.I2C_GPIO_OUT));
    hi2c->Init.I2C_GPIO_OUT.Pin = hi2c->I2C_SDA_PIN;
    HAL_GPIO_Init(hi2c->I2C_SDA_PORT, &(hi2c->Init.I2C_GPIO_OUT));
    //初始化默认拉高引脚
    HAL_GPIO_WritePin(hi2c->I2C_SCL_PORT, hi2c->I2C_SCL_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(hi2c->I2C_SDA_PORT, hi2c->I2C_SDA_PIN, GPIO_PIN_SET);
}

/**
 * @brief 改变I2C_SDA引脚模式
 * 
 * @param hi2c I2C句柄
 * @param I2C_Mode 模式
 */
void I2C_Mode(I2C_HandleTypeDef *hi2c, bool I2C_Mode)
{
    hi2c->I2C_Mode = I2C_Mode;
    if(hi2c->I2C_Mode == I2C_MODE_OUT)  //输出模式
    {
        hi2c->Init.I2C_GPIO_OUT.Pin = hi2c->I2C_SDA_PIN;
        HAL_GPIO_Init(hi2c->I2C_SDA_PORT, &(hi2c->Init.I2C_GPIO_OUT));
    }
    else  //hi2c->I2C_Mode == I2C_MODE_IN
    {
        hi2c->Init.I2C_GPIO_IN.Pin = hi2c->I2C_SDA_PIN;
        HAL_GPIO_Init(hi2c->I2C_SDA_PORT, &(hi2c->Init.I2C_GPIO_IN));
    }
}

/**
 * @brief I2C总线写入引脚状态
 * 
 * @param hi2c I2C句柄
 * @param I2C_PIN I2C引脚
 * @param Status I2C引脚状态
 */
void I2C_Write(I2C_HandleTypeDef *hi2c, bool I2C_PIN, GPIO_PinState Status)
{
    if(I2C_PIN == I2C_PIN_SCL)
        HAL_GPIO_WritePin(hi2c->I2C_SCL_PORT, hi2c->I2C_SCL_PIN, Status);
    else
        HAL_GPIO_WritePin(hi2c->I2C_SDA_PORT, hi2c->I2C_SDA_PIN, Status);
}

/**
 * @brief I2C总线读取引脚状态
 * 
 * @param hi2c I2C句柄
 * @param I2C_PIN I2C引脚
 * @return GPIO_PinState I2C引脚状态
 */
GPIO_PinState I2C_Read(I2C_HandleTypeDef *hi2c, bool I2C_PIN)
{
    if(I2C_PIN == I2C_PIN_SCL)
        return HAL_GPIO_ReadPin(hi2c->I2C_SCL_PORT, hi2c->I2C_SCL_PIN);
    else
        return HAL_GPIO_ReadPin(hi2c->I2C_SDA_PORT, hi2c->I2C_SDA_PIN);
}

/**
 * @brief 产生I2C开始信号
 * 
 * @param hi2c I2C句柄
 */
void I2C_Start(I2C_HandleTypeDef *hi2c)
{
    I2C_Mode(hi2c, I2C_MODE_OUT);  //SDA线输出模式
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_SET);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
    Delay_us(4);
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_RESET);  //开始信号(当SCL高电平，SDA下降沿)
    Delay_us(4);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);  //钳住I2C总线，准备发送/接收数据
}

/**
 * @brief 产生I2C停止信号
 * 
 * @param hi2c I2C句柄
 */
void I2C_Stop(I2C_HandleTypeDef *hi2c)
{
    I2C_Mode(hi2c, I2C_MODE_OUT);  //SDA线输出模式
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_RESET);  //停止信号(当SCL高电平，SDA上升沿)
    Delay_us(4);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_SET);  //发送I2C总线结束信号
    Delay_us(4);
}

/**
 * @brief I2C发送一个字节
 * 
 * @param hi2c I2C句柄
 * @param TxData 发送的数据
 */
void I2C_Send_Byte(I2C_HandleTypeDef *hi2c, uint8_t TxData)
{
    GPIO_PinState I2C_PIN_SDA_Status;
    I2C_Mode(hi2c, I2C_MODE_OUT);  //SDA输出模式
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);  //拉低SCL开始传输数据
    for(uint8_t nCount=0; nCount<8; nCount++)  //依次发送数据
    {
        if(((TxData&0x80)>>7) == 1)
            I2C_PIN_SDA_Status = GPIO_PIN_SET;
        else
            I2C_PIN_SDA_Status = GPIO_PIN_RESET;
        I2C_Write(hi2c, I2C_PIN_SDA, I2C_PIN_SDA_Status);
        TxData <<= 1;
        Delay_us(2);
        I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
        Delay_us(2);
        I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
        Delay_us(2);
    }
}

/**
 * @brief I2C读取1个字节，ACK=1:发送AKC，ACK=0:发送NAck
 * 
 * @param hi2c I2C句柄
 * @param Ack 是否发送Ack
 * @return uint8_t 读取的数据
 */
uint8_t I2C_Read_Byte(I2C_HandleTypeDef *hi2c, unsigned char Ack)
{
    unsigned char RxData = 0;
    I2C_Mode(hi2c, I2C_MODE_IN);  //SDA输入模式
    for(uint8_t nCount=0; nCount<8; nCount++)
    {
        I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
        Delay_us(2);
        I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
        RxData <<= 1;
        if(I2C_Read(hi2c, I2C_PIN_SDA) == GPIO_PIN_SET)
            RxData++;
        Delay_us(1);
    }
    if(!Ack)
        I2C_NAck(hi2c);  //发送NAck
    else
        I2C_Ack(hi2c);  //发送Ack
    return RxData;
}

/**
 * @brief 等待Ack信号
 * 
 * @param hi2c I2C句柄
 * @return true 接收成功
 * @return false 接收失败
 */
bool I2C_Wait_Ack(I2C_HandleTypeDef *hi2c)
{
    uint8_t ErrorTime = 0;
    I2C_Mode(hi2c, I2C_MODE_IN);  //SDA输入模式
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_SET);
    Delay_us(1);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
    Delay_us(1);
    while(I2C_Read(hi2c, I2C_PIN_SDA) == GPIO_PIN_SET)
    {
        ErrorTime++;
        if(ErrorTime > 250)
        {
            I2C_Stop(hi2c);
            return false;  //接收失败
        }
    }
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
    return true;  //接收成功
}

/**
 * @brief I2C发送Ack信号
 * 
 * @param hi2c I2C句柄
 */
void I2C_Ack(I2C_HandleTypeDef *hi2c)
{
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
    I2C_Mode(hi2c, I2C_MODE_OUT);
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_RESET);
    Delay_us(2);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
    Delay_us(2);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
}

/**
 * @brief I2C发送NAck信号
 * 
 * @param hi2c I2C句柄
 */
void I2C_NAck(I2C_HandleTypeDef *hi2c)
{
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
    I2C_Mode(hi2c, I2C_MODE_OUT);
    I2C_Write(hi2c, I2C_PIN_SDA, GPIO_PIN_SET);
    Delay_us(2);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_SET);
    Delay_us(2);
    I2C_Write(hi2c, I2C_PIN_SCL, GPIO_PIN_RESET);
}
