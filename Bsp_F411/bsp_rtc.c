#include "bsp_rtc.h"

RTC_TimeTypeDef YMX_Time;
RTC_DateTypeDef YMX_Date;
RTC_TimeTypeDef YMX_Time_Set;
RTC_DateTypeDef YMX_Date_Set;

/**
 * @brief ��ʼ��RTC
 * 
 */
void RTC_Init(void)
{
    //����ʱ��
    YMX_Time_Set.Hours = 14;
    YMX_Time_Set.Minutes = 11;
    YMX_Time_Set.Seconds = 0;
    HAL_RTC_SetTime(&hrtc, &YMX_Time_Set, RTC_FORMAT_BIN);
    //��������
    YMX_Date_Set.Year = 22;
    YMX_Date_Set.Month = 6;
    YMX_Date_Set.Date = 7;
    YMX_Date_Set.WeekDay = RTC_WEEKDAY_TUESDAY;
    HAL_RTC_SetDate(&hrtc, &YMX_Date_Set, RTC_FORMAT_BIN);
}

/**
 * @brief ����RTCʱ��
 * 
 * @param Hours ʱ��
 * @param Minutes ����
 * @param Seconds ����
 */
void RTC_SetTime(uint8_t Hours, uint8_t Minutes, uint8_t Seconds)
{
    YMX_Time_Set.Hours = Hours;
    YMX_Time_Set.Minutes = Minutes;
    YMX_Time_Set.Seconds = Seconds;
    HAL_RTC_SetTime(&hrtc, &YMX_Time_Set, RTC_FORMAT_BIN);
}

/**
 * @brief ����RTC����
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
 * @brief ��ȡRTCʱ��
 * 
 * @return RTC_TimeTypeDef ʱ��ṹ��
 */
RTC_TimeTypeDef RTC_GetTime(void)
{
    HAL_RTC_GetTime(&hrtc, &YMX_Time, RTC_FORMAT_BIN);
    return YMX_Time;
}

/**
 * @brief ��ȡ����
 * 
 * @return RTC_DateTypeDef ���ڽṹ��
 */
RTC_DateTypeDef RTC_GetDate(void)
{
    HAL_RTC_GetDate(&hrtc, &YMX_Date, RTC_FORMAT_BIN);
    return YMX_Date;
}

/**
 * @brief ˢ��RTCʱ��(��ȡĿǰ��RTC������ʱ�䵽YMX_Time�ṹ��)
 * 
 */
void RTC_RefreshTime(void)
{
    YMX_Time = RTC_GetTime();
    YMX_Date = RTC_GetDate();
}

/**
 * @brief ��ʱ������ͨ�����ڴ�ӡ
 * 
 */
void RTC_PrintTime(void)
{
    //ˢ��RTCʱ������
    RTC_RefreshTime();
    //���ڴ�ӡʱ������
    printf("/------------STM32_RTC------------/\r\n");
    printf("����ʱ�䣺%d��%d��%d�� ", YMX_Date.Year, YMX_Date.Month, YMX_Date.Date);
    printf("%d:%d:%d ", YMX_Time.Hours, YMX_Time.Minutes, YMX_Time.Seconds);
    switch(YMX_Date.WeekDay)
    {
        case RTC_WEEKDAY_MONDAY:   printf("����һ\r\n");break;
        case RTC_WEEKDAY_TUESDAY:  printf("���ڶ�\r\n");break;
        case RTC_WEEKDAY_WEDNESDAY:printf("������\r\n");break;
        case RTC_WEEKDAY_THURSDAY: printf("������\r\n");break;
        case RTC_WEEKDAY_FRIDAY:   printf("������\r\n");break;
        case RTC_WEEKDAY_SATURDAY: printf("������\r\n");break;
        case RTC_WEEKDAY_SUNDAY:   printf("������\r\n");break;
    }
    printf("\r\n");
}
