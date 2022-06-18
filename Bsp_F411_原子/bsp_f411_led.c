#include "bsp_f411_led.h"

//不带点数字表：
uint8_t Number0[] = {1,1,1,1,1,1,0,0};
uint8_t Number1[] = {0,1,1,0,0,0,0,0};
uint8_t Number2[] = {1,1,0,1,1,0,1,0};
uint8_t Number3[] = {1,1,1,1,0,0,1,0};
uint8_t Number4[] = {0,1,1,0,0,1,1,0};
uint8_t Number5[] = {1,0,1,1,0,1,1,0};
uint8_t Number6[] = {1,0,1,1,1,1,1,0};
uint8_t Number7[] = {1,1,1,0,0,0,0,0};
uint8_t Number8[] = {1,1,1,1,1,1,1,0};
uint8_t Number9[] = {1,1,1,1,0,1,1,0};
//带点数字表：
uint8_t Number0P[] = {1,1,1,1,1,1,0,1};
uint8_t Number1P[] = {0,1,1,0,0,0,0,1};
uint8_t Number2P[] = {1,1,0,1,1,0,1,1};
uint8_t Number3P[] = {1,1,1,1,0,0,1,1};
uint8_t Number4P[] = {0,1,1,0,0,1,1,1};
uint8_t Number5P[] = {1,0,1,1,0,1,1,1};
uint8_t Number6P[] = {1,0,1,1,1,1,1,1};
uint8_t Number7P[] = {1,1,1,0,0,0,0,1};
uint8_t Number8P[] = {1,1,1,1,1,1,1,1};
uint8_t Number9P[] = {1,1,1,1,0,1,1,1};

/**
 * @brief 控制F411开发板单个板载LED灯
 * 
 * @param LED_Position LED灯位置(1-8)
 * @param LED_Status LED灯状态
 */
void F411_LED_Single(uint8_t LED_Position, GPIO_PinState LED_Status)
{
    switch(LED_Position-1)
    {
        case 0:HAL_GPIO_WritePin(LED_00_GPIO_Port, LED_00_Pin, LED_Status);break;
        case 1:HAL_GPIO_WritePin(LED_01_GPIO_Port, LED_01_Pin, LED_Status);break;
        case 2:HAL_GPIO_WritePin(LED_02_GPIO_Port, LED_02_Pin, LED_Status);break;
        case 3:HAL_GPIO_WritePin(LED_03_GPIO_Port, LED_03_Pin, LED_Status);break;
        case 4:HAL_GPIO_WritePin(LED_04_GPIO_Port, LED_04_Pin, LED_Status);break;
        case 5:HAL_GPIO_WritePin(LED_05_GPIO_Port, LED_05_Pin, LED_Status);break;
        case 6:HAL_GPIO_WritePin(LED_06_GPIO_Port, LED_06_Pin, LED_Status);break;
        case 7:HAL_GPIO_WritePin(LED_07_GPIO_Port, LED_07_Pin, LED_Status);break;
    }
}

/**
 * @brief 控制F411开发板全部板载LED灯
 * 
 * @param LED_Status 2进制LED灯状态(1点亮0熄灭)
 */
void F411_LED(long LED_Status)
{
    for(uint8_t nCount=0; nCount<8; nCount++)
    {
        if(LED_Status%10 == 0)  //熄灭
            F411_LED_Single(nCount, GPIO_PIN_SET);
        else  //点亮
            F411_LED_Single(nCount, GPIO_PIN_RESET);
        LED_Status /= 10;
    }
}

/**
 * @brief 控制F411开发板单个板载数码管显示数字
 * 
 * @param SEG_Position 数码管位置(1-8)
 * @param SEG_Show 数码管显示数字
 * @param Point 是否显示小数点
 */
void F411_SEG_Single(uint8_t SEG_Position, uint8_t SEG_Show, bool Point)
{
    //段选:
    HC138(SEG_Position-1);  //从1开始编号
    //位选：
    if(Point == 0)
    {
        switch(SEG_Show)
        {
            case 0:HC595(Number0);break;
            case 1:HC595(Number1);break;
            case 2:HC595(Number2);break;
            case 3:HC595(Number3);break;
            case 4:HC595(Number4);break;
            case 5:HC595(Number5);break;
            case 6:HC595(Number6);break;
            case 7:HC595(Number7);break;
        }
    }
    else  //Point == 1
    {
        switch(SEG_Show)
        {
            case 0:HC595(Number0P);break;
            case 1:HC595(Number1P);break;
            case 2:HC595(Number2P);break;
            case 3:HC595(Number3P);break;
            case 4:HC595(Number4P);break;
            case 5:HC595(Number5P);break;
            case 6:HC595(Number6P);break;
            case 7:HC595(Number7P);break;
        }
    }
}

/**
 * @brief 控制F411开发板全部板载数码管显示数字(仅刷新一次，需要持续刷新)
 * 
 * @param SEG_Show 数码管显示数字
 * @param Point_Positon 小数点位置
 */
void F411_SEG(long SEG_Show, uint8_t Point_Positon)
{
    for(uint8_t nCount=1; nCount<9; nCount++)  //依次点亮数码管
    {
        if(nCount != Point_Positon)
            F411_SEG_Single(nCount, SEG_Show%10, 0);
        else  //刷新到需要显示小数点的数码管
            F411_SEG_Single(nCount, SEG_Show%10, 1);
        SEG_Show /= 10;
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
    Time = Time << 2;
    Time += YMX_Time.Minutes;
    Time = Time << 2;
    Time += YMX_Time.Seconds;
    F411_SEG(Time, Point_Position_None);
}
#endif

/***************************************************/
/********************芯片操作函数********************/
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

/**
 * @brief 高电平有效
 * 
 * @param Number 数字编码
 * @param Point 是否点亮小数点
 */
void HC595(uint8_t NumberCode[])
{
    HC595_LCLK(GPIO_PIN_RESET);  //复位
    for(uint8_t nCount=0; nCount<8; nCount++)  //向寄存器写入数据
    {
        if(NumberCode[nCount] == 0)
            HC595_DATA(GPIO_PIN_RESET);
        else
            HC595_DATA(GPIO_PIN_SET);
        HC595_SCLK(GPIO_PIN_RESET);  //上升沿写入数据
        YMX_Delay(10);
        HC595_SCLK(GPIO_PIN_SET);  //复位
    }
    HC595_LCLK(GPIO_PIN_SET);  //上升沿输出数据
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
