#include "bsp_f411_tpad.h"

//定时器3句柄
TIM_HandleTypeDef htim3 = {0};

//TPAD按键悬空默认值：
uint16_t TPAD_Default_Value = 0;

/**
 * @brief 初始化TPAD按键
 * 
 * @param PSC TIM3分频系数实际值(不需要-1)，越小灵敏度越高(F411=96Mhz)
 * @return true 初始化成功
 * @return false 初始化失败
 */
bool F411_TPAD_Init(uint16_t PSC)
{
    uint16_t Buffer[10];  //存储10次采样值
    uint16_t Temp = 0;  //排序缓存
    uint8_t aCount = 0, bCount = 0;
    F411_TPAD_TIM3_Init(TPAD_ARR_MAX, PSC-1);  //初始化TIM
    for(aCount=0; aCount<10; aCount++)  //获取10次采样值
    {
        Buffer[aCount] = F411_TPAD_Get_Value();
        osDelay(10);
    }
    for(aCount=0; aCount<9; aCount++)  //采样值排序
    {
        for(bCount=aCount+1; bCount<10; bCount++)  //升序排序
        {
            Temp = Buffer[aCount];
            Buffer[aCount] = Buffer[bCount];
            Buffer[bCount] = Temp;
        }
    }
    Temp = 0;
    for(aCount=2; aCount<8; aCount++)  //去掉2个高值，去掉2个低值
        Temp += Buffer[aCount];
    TPAD_Default_Value = Temp/6;  //取平均值
    printf("TPAD_Defaul_Value = %d\r\n", TPAD_Default_Value);
    if(TPAD_Default_Value > ((uint16_t)TPAD_ARR_MAX/2))  //初始化值>TPAD_ARR_MAX/2，初始化失败
        return false;
    else  //初始化成功
        return true;
}

/**
 * @brief 扫描TPAD
 * 
 * @param Mode 1：支持连续触发，0：单次触发(按下后必须松开一次)
 * @return true 检测到按下
 * @return false 未检测到按下
 */
bool F411_TPAD_Scan(bool Mode)
{
    static uint8_t TPAD_Enable = 0;  //0：可以开始检测，!0：还不能检测
    bool Touched = false;
    uint8_t Sample_Times = 3;  //默认采样3次
    uint16_t TPAD_Value = 0;  //获取到的采样值
    if(Mode)
    {
        Sample_Times = 6;  //支持连按时，采样次数为6
        TPAD_Enable = 0;  //支持连按
    }
    TPAD_Value = F411_TPAD_Get_MaxValue(Sample_Times);
    if(TPAD_Value > (TPAD_Default_Value+TPAD_Gate_Value))
    {
        if(TPAD_Enable == 0)  //TPAD_Enable == 0，有效
            Touched = true;
        TPAD_Enable = 3;  //至少要再过3次后才能有效
    }
    if(TPAD_Enable)
        TPAD_Enable--;
    else
        return Touched;
    return false;  //避免编译器报警
}

/**
 * @brief TPAD复位(释放电容，清除定时器数值)
 * 
 */
void F411_TPAD_Reset(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Pin = TPAD_GPIO_PIN;
    HAL_GPIO_Init(TPAD_GPIO_PORT, &GPIO_InitStructure);

    HAL_GPIO_WritePin(TPAD_GPIO_PORT, TPAD_GPIO_PIN, GPIO_PIN_RESET);
    osDelay(5);
    __HAL_TIM_CLEAR_FLAG(&htim3, TIM_FLAG_CC4|TIM_FLAG_UPDATE);
    __HAL_TIM_SET_COUNTER(&htim3, 0);

    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(TPAD_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 读取定时器捕获值
 * 
 * @return uint16_t 捕获值(超时直接返回计数值)
 */
uint16_t F411_TPAD_Get_Value(void)
{
    F411_TPAD_Reset();  //释放电容
    while(__HAL_TIM_GET_FLAG(&htim3, TIM_FLAG_CC4) == RESET)
    {
        if(__HAL_TIM_GET_COUNTER(&htim3) > TPAD_ARR_MAX-500)  //超时直接返回计数值
            return __HAL_TIM_GET_COUNTER(&htim3);
    }
    return HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_4);  //返回TIM3.CH4捕获值
}

/**
 * @brief 获取TImes次，取最大值
 * 
 * @param Times 获取次数
 * @return uint16_t 最大值
 */
uint16_t F411_TPAD_Get_MaxValue(uint8_t Times)
{
    uint16_t Temp = 0;
    uint16_t Max = 0;
    uint8_t Touch_Gate = (Times*2)/3;  //至少需要2/3个有效触摸才有效
    uint8_t Touch_OK = 0;
    while(Times--)
    {
        Temp = F411_TPAD_Get_Value();  //得到一次捕获值
        if(Temp > (TPAD_Default_Value*5 / 4))  //大于默认值的5/4算有效
            Touch_OK++;
        if(Temp > Max)  //获取到更大值
            Max = Temp;
    }
    if(Touch_OK >= Touch_Gate)  //至少有2/3的采样值 > 默认值的5/4算作有效采样
        return Max;
    else
        return 0;
}

/**
 * @brief 初始化TIM3
 * 
 * @param ARR 重装载寄存器值
 * @param PSC 预分频系数
 */
void F411_TPAD_TIM3_Init(uint32_t ARR, uint16_t PSC)
{
    TIM_IC_InitTypeDef htim3_ch4;

    htim3.Instance = TIM3;  //定时器3
    htim3.Init.Period = ARR;  //重装载寄存器值
    htim3.Init.Prescaler = PSC;  //预分频系数
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;  //向上计数
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  //不分频
    HAL_TIM_IC_Init(&htim3);

    htim3_ch4.ICPolarity = TIM_ICPOLARITY_RISING;  //上升沿捕获
    htim3_ch4.ICSelection = TIM_ICSELECTION_DIRECTTI;  //映射到TI1
    htim3_ch4.ICPrescaler = TIM_ICPSC_DIV1;  //不分频
    htim3_ch4.ICFilter = 0;  //不滤波
    HAL_TIM_IC_ConfigChannel(&htim3, &htim3_ch4, TIM_CHANNEL_4);  //配置TIM3.CH4
    HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_4);  //开始捕获TIM3.CH4
}

/**
 * @brief TIM3底层驱动，时钟使能，引脚配置(此函数会被HAL_TIM_IC_Init()自动调用)
 * 
 * @param htim 定时器句柄
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_TIM3_CLK_ENABLE();  //使能TIM3时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();  //使能GPIOB时钟

    GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;  //复用推挽模式
    GPIO_InitStructure.Pull = GPIO_NOPULL;  //不拉
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;  //高速模式
    GPIO_InitStructure.Pin = TPAD_GPIO_PIN;
    GPIO_InitStructure.Alternate = GPIO_AF2_TIM3;  //复用为TIM3.CH4
    HAL_GPIO_Init(TPAD_GPIO_PORT, &GPIO_InitStructure);
}
