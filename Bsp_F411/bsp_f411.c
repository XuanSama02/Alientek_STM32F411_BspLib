#include "bsp_f411.h"

/**
 * @brief F411开发板测试一条龙
 * 
 */
void F411_Test(void)
{
    printf("\r\n");
    printf("F411 Test Starting:\r\n");
    F411_Test_W25Q16();
    F411_Test_AT24C02();
    F411_Test_TPAD();  //注意此测试为无限循环
}

/**
 * @brief 测试板载SPI Flash
 * 
 */
void F411_Test_W25Q16(void)
{
    //初始化测试变量
    uint8_t TEST[] = {"W25Q16 TEST Success!"};
    uint8_t Read[sizeof(TEST)];
    uint16_t ID = W25QXX_ReadID();
    printf("F411_TEST: W25Q16\r\n");
    osDelay(1000);
    //初始化硬件
    if((ID==0) || (ID==0xFFFF))
        printf("Read ID Failed!\r\n");
    else
        printf("Flash ID: %#x\r\n", ID);
    osDelay(1000);
    //运行测试
    W25QXX_Write((2*1024*1024)-100, (uint8_t*)TEST, sizeof(TEST));
    W25QXX_Read((2*1024*1024)-100, Read, sizeof(TEST));
    printf("%s\r\n", Read);
    printf("\r\n");
    osDelay(2000);
}

/**
 * @brief 测试板载EEPROM
 * 
 */
void F411_Test_AT24C02(void)
{
    //初始化测试变量
    uint8_t TEST[] = {"AT24C02 TEST Success!"};
    uint8_t Read[sizeof(TEST)];
    printf("F411_TEST: AT24C02\r\n");
    osDelay(1000);
    //初始化硬件
    if(!AT24CXX_Check())
        printf("AT24C02 Init Failed!\r\n");
    else
        printf("AT24C02 Init Success!\r\n");
    osDelay(1000);
    //运行测试
    AT24CXX_Write(0, (uint8_t*)TEST, sizeof(TEST));
    AT24CXX_Read(0, Read, sizeof(TEST));
    printf("%s\r\n", Read);
    printf("\r\n");
    osDelay(2000);
}

/**
 * @brief 测试板载TouchPad(无限循环)
 * 
 */
void F411_Test_TPAD(void)
{
    F411_TPAD_Init(6);
    printf("F411_TEST: TouchPad\r\n");
    osDelay(1000);
    printf("TouchPad started Mode0 for Infinity\r\n");
    while(1)
    {
        if(F411_TPAD_Scan(0))
        {
            printf("Touched!\r\n");
            HAL_GPIO_TogglePin(LED_00_GPIO_Port, LED_00_Pin);
        } 
        osDelay(1);
    }
}
