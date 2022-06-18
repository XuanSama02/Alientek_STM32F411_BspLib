#include "bsp_at24cxx.h"

uint16_t EEPROM_TYPE = 0;

/**
 * @brief 初始化AT24CXX
 * 
 * @param EEPROM_TYPE AT24CXX具体型号
 */
void AT24CXX_Init(uint16_t AT24CXX)
{
    EEPROM_TYPE = AT24CXX;  //初始化EEPROM_TYPE
}

/**
 * @brief 检查AT24CXX是否正常，地址末最后一位存储标志位
 * 
 * @return true 检测成功
 * @return false 检测失败
 */
bool AT24CXX_Check(void)
{
    uint8_t Check_Flag = 0;
    Check_Flag = AT24CXX_ReadByte(255);  //检查标志位
    if(Check_Flag == 0x55)  //检查标志位是否为0x55
        return true;
    else  //排除为第一次初始化的情况
    {
        AT24CXX_WriteByte(255, 0x55);  //标志位写入0x55
        Check_Flag = AT24CXX_ReadByte(255);
        if(Check_Flag == 0x55)
            return true;
    }
    return false;  //失败
}

/**
 * @brief 指定地址读取一个字节
 * 
 * @param Addr 读取地址
 * @return uint8_t 读取到的数据
 */
uint8_t AT24CXX_ReadByte(uint16_t Addr)
{
    uint8_t Data = 0;
    I2C_Start();
    if(EEPROM_TYPE > AT24C16)  //容量>16Mbit
    {
        I2C_Send_Byte(0xA0);  //写命令
        I2C_Wait_Ack();
        I2C_Send_Byte(Addr>>8);  //发送高地址
    }
    else  //容量<=16Mbit
        I2C_Send_Byte(0xA0 + ((Addr/256)<<1));
    I2C_Wait_Ack();
    I2C_Send_Byte(Addr%256);  //发送低地址
    I2C_Wait_Ack();
    I2C_Start();
    I2C_Send_Byte(0xA1);  //进入接收模式
    I2C_Wait_Ack();
    Data = I2C_Read_Byte(0);
    I2C_Stop();  //停止信号
    return Data;
}

/**
 * @brief 指定地址读取指定长度数据(适用于16bit/32bit数据)
 * 
 * @param Addr 读取地址
 * @param Length 读取长度2,4
 * @return uint32_t 读取到的数据
 */
uint32_t AT24CXX_ReadByte_Length(uint16_t Addr, uint8_t Length)
{
    uint32_t Data = 0;
    for(uint8_t nCount=0; nCount<Length; nCount++)
    {
        Data <<= 8;
        Data += AT24CXX_ReadByte(Addr + (Length-nCount-1));
    }
    return Data;
}

/**
 * @brief 从指定地址开始读取指定长度数据
 * 
 * @param Addr 读取地址(0-EEPROM_TYPE)
 * @param pBuffer 数据数组首地址
 * @param NumToRead 要读取的数据个数
 */
void AT24CXX_Read(uint16_t Addr, uint8_t* pBuffer, uint16_t NumToRead)
{
    while(NumToRead--)
        *pBuffer ++= AT24CXX_ReadByte(Addr++);
}

/**
 * @brief 指定地址写入一个字节
 * 
 * @param Addr 写入地址
 * @param Data 写入数据
 */
void AT24CXX_WriteByte(uint16_t Addr, uint8_t Data)
{
    I2C_Start();
    if(EEPROM_TYPE > AT24C16)
    {
        I2C_Send_Byte(0xA0);  //发送写命令
        I2C_Wait_Ack();
        I2C_Send_Byte(Addr>>8);  //发送高地址
    }
    else
    {
        I2C_Send_Byte(0xA0 + ((Addr/256)<<1));  //发送器件地址
    }
    I2C_Wait_Ack();
    I2C_Send_Byte(Addr%256);  //发送低地址
    I2C_Wait_Ack();
    I2C_Send_Byte(Data);  //发送数据
    I2C_Wait_Ack();
    I2C_Stop();  //停止信号
    osDelay(10);
}

/**
 * @brief 指定地址写入指定长度数据(适用于16bit/32bit数据)
 * 
 * @param Addr 写入地址
 * @param Data 写入数据
 * @param Length 数据长度2,4
 */
void AT24CXX_WriteByte_Length(uint16_t Addr, uint32_t Data, uint8_t Length)
{
    for(uint8_t nCount=0; nCount<Length; nCount++)
        AT24CXX_WriteByte(Addr+nCount, (Data >> (8*nCount)) & 0xFF);
}

/**
 * @brief 从指定地址开始写入指定长度数据
 * 
 * @param Addr 写入地址(0-EEPROM_TYPE)
 * @param pBuffer 数据数组首地址
 * @param NumToWrite 要写入的数据个数
 */
void AT24CXX_Write(uint16_t Addr, uint8_t* pBuffer, uint16_t NumToWrite)
{
    while(NumToWrite--)
    {
        AT24CXX_WriteByte(Addr, *pBuffer);
        Addr++;
        pBuffer++;
    }
}
