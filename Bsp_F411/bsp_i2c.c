#include "bsp_i2c.h"

/**
 * @brief 改变SDA线模式
 * 
 * @param Mode 目标模式(IN-OUT)
 */
void SDA_Mode(bool Mode)
{
    if(Mode == IN)  //输入模式
    {
        GPIOB->MODER &= ~(3<<(7*2));
        GPIOB->MODER |= 0<<(7*2);
    }
    else  //输出模式
    {
        GPIOB->MODER &= ~(3<<(7*2));
        GPIOB->MODER |= 1<<(7*2);
    }
}

/**
 * @brief 写入SCL线
 * 
 * @param Status SCL引脚状态
 */
void I2C_SCL_Write(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(I2C_SCL_GPIO_Port, I2C_SCL_Pin, Status);
}

/**
 * @brief 写入SDA线
 * 
 * @param Status SDA引脚状态
 */
void I2C_SDA_Write(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(I2C_SDA_GPIO_Port, I2C_SDA_Pin, Status);
}

/**
 * @brief 读取SCL线状态
 * 
 * @return GPIO_PinState SCL引脚状态
 */
GPIO_PinState I2C_SCL_Read(void)
{
    return HAL_GPIO_ReadPin(I2C_SCL_GPIO_Port, I2C_SCL_Pin);
}

/**
 * @brief 读取SDA线状态
 * 
 * @return GPIO_PinState SDA引脚状态
 */
GPIO_PinState I2C_SDA_Read(void)
{
    return HAL_GPIO_ReadPin(I2C_SDA_GPIO_Port, I2C_SDA_Pin);
}

/**
 * @brief 产生I2C开始信号
 * 
 */
void I2C_Start(void)
{
    SDA_Mode(OUT);  //SDA线输出模式
    I2C_SDA_Write(GPIO_PIN_SET);
    I2C_SCL_Write(GPIO_PIN_SET);
    Delay_us(4);
    I2C_SDA_Write(GPIO_PIN_RESET);  //开始信号(当SCL高电平，SDA下降沿)
    Delay_us(4);
    I2C_SCL_Write(GPIO_PIN_RESET);  //钳住I2C总线，准备发送/接收数据
}

/**
 * @brief 产生I2C停止信号
 * 
 */
void I2C_Stop(void)
{
    SDA_Mode(OUT);  //SDA线输出模式
    I2C_SCL_Write(GPIO_PIN_RESET);
    I2C_SDA_Write(GPIO_PIN_RESET);  //停止信号(当SCL高电平，SDA上升沿)
    Delay_us(4);
    I2C_SCL_Write(GPIO_PIN_SET);
    I2C_SDA_Write(GPIO_PIN_SET);  //发送I2C总线结束信号
    Delay_us(4);
}

/**
 * @brief I2C发送1个字节
 * 
 * @param TxData 发送的数据
 */
void I2C_Send_Byte(uint8_t TxData)
{
    GPIO_PinState SDA_Status;
    SDA_Mode(OUT);  //SDA输出模式
    I2C_SCL_Write(GPIO_PIN_RESET);  //拉低SCL开始传输数据
    for(uint8_t nCount=0; nCount<8; nCount++)  //依次发送数据
    {
        if(((TxData&0x80)>>7) == 1)
            SDA_Status = GPIO_PIN_SET;
        else
            SDA_Status = GPIO_PIN_RESET;
        I2C_SDA_Write(SDA_Status);
        TxData <<= 1;
        Delay_us(2);
        I2C_SCL_Write(GPIO_PIN_SET);
        Delay_us(2);
        I2C_SCL_Write(GPIO_PIN_RESET);
        Delay_us(2);
    }
}

/**
 * @brief I2C读取1个字节，ACK=1:发送AKC，ACK=0:发送NAck
 * 
 * @param Ack 是否发送Ack
 * @return uint8_t 读取的数据
 */
uint8_t I2C_Read_Byte(unsigned char Ack)
{
    unsigned char RxData = 0;
    SDA_Mode(IN);  //SDA输入模式
    for(uint8_t nCount=0; nCount<8; nCount++)
    {
        I2C_SCL_Write(GPIO_PIN_RESET);
        Delay_us(2);
        I2C_SCL_Write(GPIO_PIN_SET);
        RxData <<= 1;
        if(I2C_SDA_Read() == GPIO_PIN_SET)
            RxData++;
        Delay_us(1);
    }
    if(!Ack)
        I2C_NAck();  //发送NAck
    else
        I2C_Ack();  //发送Ack
    return RxData;
}

/**
 * @brief 等待Ack信号
 * 
 * @return true 接收成功
 * @return false 接收失败
 */
bool I2C_Wait_Ack(void)
{
    uint8_t ErrorTime = 0;
    SDA_Mode(IN);  //SDA输入模式
    I2C_SDA_Write(GPIO_PIN_SET);
    Delay_us(1);
    I2C_SCL_Write(GPIO_PIN_SET);
    Delay_us(1);
    while(I2C_SDA_Read() == GPIO_PIN_SET)
    {
        ErrorTime++;
        if(ErrorTime > 250)
        {
            I2C_Stop();
            return false;  //接收失败
        }
    }
    I2C_SCL_Write(GPIO_PIN_RESET);
    return true;  //接收成功
}

/**
 * @brief I2C发送Ack信号
 * 
 */
void I2C_Ack(void)
{
    I2C_SCL_Write(GPIO_PIN_RESET);
    SDA_Mode(OUT);
    I2C_SDA_Write(GPIO_PIN_RESET);
    Delay_us(2);
    I2C_SCL_Write(GPIO_PIN_SET);
    Delay_us(2);
    I2C_SCL_Write(GPIO_PIN_RESET);
}

/**
 * @brief I2C发送NAck信号
 * 
 */
void I2C_NAck(void)
{
    I2C_SCL_Write(GPIO_PIN_RESET);
    SDA_Mode(OUT);
    I2C_SDA_Write(GPIO_PIN_SET);
    Delay_us(2);
    I2C_SCL_Write(GPIO_PIN_SET);
    Delay_us(2);
    I2C_SCL_Write(GPIO_PIN_RESET);
}
