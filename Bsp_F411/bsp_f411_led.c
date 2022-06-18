#include "bsp_f411_led.h"

uint8_t SEG_Special[] = {0x00, 0x01};  //全黑，小数点
uint8_t SEG_NumP[] = {0xFD, 0x61, 0xDB, 0xF3, 0x67, 0xB7, 0xBF, 0xE1, 0xFF, 0xF7};
uint8_t SEG_Num[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6};
//uint8_t SEG_Eng[] = {0xEE, 0x3E, 0x9C, 0x7A, 0x9E, 0x8E};

//全局变量存储数码管显示值
static uint8_t SEG[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static uint8_t Point = Point_Position_None;

/**
 * @brief 控制F411开发板全部板载LED灯
 * 
 * @param LED_Status 2进制LED灯状态(1点亮0熄灭)
 */
void F411_LED(long LED_Status)
{
    bool LED[8];
    LED[0] = LED_Status/10000000;
    LED[1] = (LED_Status/1000000)%10;
    LED[2] = (LED_Status/100000)%10;
    LED[3] = (LED_Status/10000)%10;
    LED[4] = (LED_Status/1000)%10;
    LED[5] = (LED_Status/100)%10;
    LED[6] = (LED_Status/10)%10;
    LED[7] = LED_Status%10;
    for(uint8_t nCount=1; nCount<9; nCount++)
    {
        if(LED[nCount-1])  //熄灭
            F411_LED_Single(nCount, OFF);
        else  //点亮
            F411_LED_Single(nCount, ON);
        LED_Status = LED_Status/10;
    }
}

/**
 * @brief 控制F411开发板单个板载LED灯
 * 
 * @param LED_Position LED灯位置(1-8)
 * @param LED_Status LED灯状态(ON-OFF)
 */
void F411_LED_Single(uint8_t LED_Position, bool LED_Status)
{
    switch(LED_Position-1)
    {
        case 0:HAL_GPIO_WritePin(LED_00_GPIO_Port, LED_00_Pin, (GPIO_PinState)!LED_Status);break;
        case 1:HAL_GPIO_WritePin(LED_01_GPIO_Port, LED_01_Pin, (GPIO_PinState)!LED_Status);break;
        case 2:HAL_GPIO_WritePin(LED_02_GPIO_Port, LED_02_Pin, (GPIO_PinState)!LED_Status);break;
        case 3:HAL_GPIO_WritePin(LED_03_GPIO_Port, LED_03_Pin, (GPIO_PinState)!LED_Status);break;
        case 4:HAL_GPIO_WritePin(LED_04_GPIO_Port, LED_04_Pin, (GPIO_PinState)!LED_Status);break;
        case 5:HAL_GPIO_WritePin(LED_05_GPIO_Port, LED_05_Pin, (GPIO_PinState)!LED_Status);break;
        case 6:HAL_GPIO_WritePin(LED_06_GPIO_Port, LED_06_Pin, (GPIO_PinState)!LED_Status);break;
        case 7:HAL_GPIO_WritePin(LED_07_GPIO_Port, LED_07_Pin, (GPIO_PinState)!LED_Status);break;
    }
}

void F411_SEG(long SEG_Show, uint8_t Point_Position)
{
    Point = Point_Position;
    SEG[7] = SEG_Show/10000000;
    SEG[6] = (SEG_Show/1000000)%10;
    SEG[5] = (SEG_Show/100000)%10;
    SEG[4] = (SEG_Show/10000)%10;
    SEG[3] = (SEG_Show/1000)%10;
    SEG[2] = (SEG_Show/100)%10;
    SEG[1] = (SEG_Show/10)%10;
    SEG[0] = SEG_Show%10;
}

void F411_SEG_Single(uint8_t SEG_Position, uint8_t SEG_Show, bool Point)
{
    uint8_t Code = 0;
    if(Point == 0)  //不显示小数点
    {
        switch(SEG_Show)
        {
            case 0:Code = SEG_Num[0];break;
            case 1:Code = SEG_Num[1];break;
            case 2:Code = SEG_Num[2];break;
            case 3:Code = SEG_Num[3];break;
            case 4:Code = SEG_Num[4];break;
            case 5:Code = SEG_Num[5];break;
            case 6:Code = SEG_Num[6];break;
            case 7:Code = SEG_Num[7];break;
            case 8:Code = SEG_Num[8];break;
            case 9:Code = SEG_Num[9];break;
        }
    }
    else  //显示小数点
    {
        switch(SEG_Show)
        {
            case 0:Code = SEG_NumP[0];break;
            case 1:Code = SEG_NumP[1];break;
            case 2:Code = SEG_NumP[2];break;
            case 3:Code = SEG_NumP[3];break;
            case 4:Code = SEG_NumP[4];break;
            case 5:Code = SEG_NumP[5];break;
            case 6:Code = SEG_NumP[6];break;
            case 7:Code = SEG_NumP[7];break;
            case 8:Code = SEG_NumP[8];break;
            case 9:Code = SEG_NumP[9];break;
        }
    }
    for(uint8_t nCount=0; nCount<8; nCount++)  //段选
    {
        if((Code>>nCount)&0x01)
            HC595_DATA(GPIO_PIN_SET);
        else
            HC595_DATA(GPIO_PIN_RESET);
        HC595_SCLK(GPIO_PIN_SET);  //上升沿有效
        HC595_SCLK(GPIO_PIN_RESET);
    }
    HC138(SEG_Position-1);  //位选
    HC595_LCLK(GPIO_PIN_SET);  //上升沿有效
    HC595_LCLK(GPIO_PIN_RESET);
}

/**
 * @brief 刷新数码管
 * 
 */
void F411_SEG_Run(uint8_t FreshSpeed)
{
    uint8_t nCount = 1;
    while(nCount<9)
    {
        if(Point == nCount)
            F411_SEG_Single(nCount, SEG[nCount-1], 1);
        else
            F411_SEG_Single(nCount, SEG[nCount-1], 0);
        osDelay(FreshSpeed);
        nCount++;
    }
}

#ifdef RTC_ENABLE
/**
 * @brief 数码管显示时间
 * 
 * @note 需要FreeRTOS
 */
void F411_SEG_RTC(void)
{
    long Time = 00000000;
    RTC_RefreshTime();
    Time += YMX_Time.Hours;
    Time = Time * 100;
    Time += YMX_Time.Minutes;
    Time = Time * 100;
    Time += YMX_Time.Seconds;
    F411_SEG(Time, Point_Position_None);
}
#endif

/***************************************************/
/********************引脚操作函数********************/
/***************************************************/

/**
 * @brief CHx代表的二进制为逻辑有效(高电平有效)，实际输出为低电平有效
 * 
 * @param CHx 低电平输出通道
 */
void HC138(uint8_t CHx)
{
    switch(CHx)
    {
        case 0:HC138_A2(GPIO_PIN_SET),   HC138_A1(GPIO_PIN_SET),   HC138_A0(GPIO_PIN_SET);   break;
        case 1:HC138_A2(GPIO_PIN_SET),   HC138_A1(GPIO_PIN_SET),   HC138_A0(GPIO_PIN_RESET); break;
        case 2:HC138_A2(GPIO_PIN_SET),   HC138_A1(GPIO_PIN_RESET), HC138_A0(GPIO_PIN_SET);   break;
        case 3:HC138_A2(GPIO_PIN_SET),   HC138_A1(GPIO_PIN_RESET), HC138_A0(GPIO_PIN_RESET); break;
        case 4:HC138_A2(GPIO_PIN_RESET), HC138_A1(GPIO_PIN_SET),   HC138_A0(GPIO_PIN_SET);   break;
        case 5:HC138_A2(GPIO_PIN_RESET), HC138_A1(GPIO_PIN_SET),   HC138_A0(GPIO_PIN_RESET); break;
        case 6:HC138_A2(GPIO_PIN_RESET), HC138_A1(GPIO_PIN_RESET), HC138_A0(GPIO_PIN_SET);   break;
        case 7:HC138_A2(GPIO_PIN_RESET), HC138_A1(GPIO_PIN_RESET), HC138_A0(GPIO_PIN_RESET); break;
    }
}

/***************************************************/
/********************引脚操作函数********************/
/***************************************************/
//74HC138
void HC138_A0(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(HC138_A0_PORT, HC138_A0_PIN, Status);
}

void HC138_A1(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(HC138_A1_PORT, HC138_A1_PIN, Status);
}

void HC138_A2(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(HC138_A2_PORT, HC138_A2_PIN, Status);
}

//74HC595
void HC595_DATA(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(HC595_DATA_PORT, HC595_DATA_PIN, Status);
}

void HC595_LCLK(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(HC595_LCLK_PORT, HC595_LCLK_PIN, Status);
}

void HC595_SCLK(GPIO_PinState Status)
{
    HAL_GPIO_WritePin(HC595_SCLK_PORT, HC595_SCLK_PIN, Status);
}
