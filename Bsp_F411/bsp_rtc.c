#include "bsp_rtc.h"

RTC_TimeTypeDef YMX_Time;
RTC_DateTypeDef YMX_Date;
RTC_TimeTypeDef YMX_Time_Set;
RTC_DateTypeDef YMX_Date_Set;

/**
 * @brief 初始化RTC
 * 
 */
void RTC_Init(void)
{
    //设置时间
    YMX_Time_Set.Hours = 14;
    YMX_Time_Set.Minutes = 11;
    YMX_Time_Set.Seconds = 0;
    HAL_RTC_SetTime(&hrtc, &YMX_Time_Set, RTC_FORMAT_BIN);
    //设置日期
    YMX_Date_Set.Year = 22;
    YMX_Date_Set.Month = 6;
    YMX_Date_Set.Date = 7;
    YMX_Date_Set.WeekDay = RTC_WEEKDAY_TUESDAY;
    HAL_RTC_SetDate(&hrtc, &YMX_Date_Set, RTC_FORMAT_BIN);
}

/**
 * @brief 设置RTC时间
 * 
 * @param Hours 时针
 * @param Minutes 分针
 * @param Seconds 秒针
 */
void RTC_SetTime(uint8_t Hours, uint8_t Minutes, uint8_t Seconds)
{
    YMX_Time_Set.Hours = Hours;
    YMX_Time_Set.Minutes = Minutes;
    YMX_Time_Set.Seconds = Seconds;
    HAL_RTC_SetTime(&hrtc, &YMX_Time_Set, RTC_FORMAT_BIN);
}

/**
 * @brief 设置RTC日期
 * 
 * @param Year 
 * @param Month 
 * @param Date 
 * @param Weekday 
 */
void RTC_SetDate(uint8_t Year, uint8_t Month, uint8_t Date, uint8_t WeekDay)
{
    YMX_Date_Set.Year = Year;
    YMX_Date_Set.Month = Month;
    YMX_Date_Set.Date = Date;
    YMX_Date_Set.WeekDay = WeekDay;
}

/**
 * @brief 获取RTC时间
 * 
 * @return RTC_TimeTypeDef 时间结构体
 */
RTC_TimeTypeDef RTC_GetTime(void)
{
    HAL_RTC_GetTime(&hrtc, &YMX_Time, RTC_FORMAT_BIN);
    return YMX_Time;
}

/**
 * @brief 获取日期
 * 
 * @return RTC_DateTypeDef 日期结构体
 */
RTC_DateTypeDef RTC_GetDate(void)
{
    HAL_RTC_GetDate(&hrtc, &YMX_Date, RTC_FORMAT_BIN);
    return YMX_Date;
}

/**
 * @brief 刷新RTC时间(获取目前的RTC日期与时间到YMX_Time结构体)
 * 
 */
void RTC_RefreshTime(void)
{
    YMX_Time = RTC_GetTime();
    YMX_Date = RTC_GetDate();
}

/**
 * @brief 将时间日期通过串口打印
 * 
 */
void RTC_PrintTime(void)
{
    //刷新RTC时间日期
    RTC_RefreshTime();
    //串口打印时间日期
    printf("/------------STM32_RTC------------/\r\n");
    printf("北京时间：%d年%d月%d日 ", YMX_Date.Year, YMX_Date.Month, YMX_Date.Date);
    printf("%d:%d:%d ", YMX_Time.Hours, YMX_Time.Minutes, YMX_Time.Seconds);
    switch(YMX_Date.WeekDay)
    {
        case RTC_WEEKDAY_MONDAY:   printf("星期一\r\n");break;
        case RTC_WEEKDAY_TUESDAY:  printf("星期二\r\n");break;
        case RTC_WEEKDAY_WEDNESDAY:printf("星期三\r\n");break;
        case RTC_WEEKDAY_THURSDAY: printf("星期四\r\n");break;
        case RTC_WEEKDAY_FRIDAY:   printf("星期五\r\n");break;
        case RTC_WEEKDAY_SATURDAY: printf("星期六\r\n");break;
        case RTC_WEEKDAY_SUNDAY:   printf("星期日\r\n");break;
    }
    printf("\r\n");
}
