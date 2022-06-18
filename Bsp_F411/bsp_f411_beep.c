#include "bsp_f411_beep.h"

/**
 * @brief 控制F411板载蜂鸣器
 * 
 * @param Status 蜂鸣器状态(ON/OFF)
 */
void F411_Beep(bool Status)
{
    if(Status == ON)
        HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(BEEP_PORT, BEEP_PIN, GPIO_PIN_SET);
}
