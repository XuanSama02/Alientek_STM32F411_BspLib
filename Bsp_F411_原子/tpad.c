#include "tpad.h"
#include "delay.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32F4开发板
//TPAD驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2019/4/23
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2019-2029
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
TIM_HandleTypeDef TIM3_Handler;         //定时器3句柄 

#define TPAD_ARR_MAX_VAL  0XFFFF		//最大的ARR值(TIM3是16位定时器)	  
vu16 tpad_default_val=0;				//空载的时候(没有手按下),计数器需要的时间

//初始化触摸按键
//获得空载的时候触摸按键的取值.
//psc:分频系数,越小,灵敏度越高.
//返回值:0,初始化成功;1,初始化失败
u8 TPAD_Init(u8 psc)
{
	u16 buf[10];
	u16 temp;
	u8 i,j;
	TIM3_CH4_Cap_Init(TPAD_ARR_MAX_VAL,psc-1);//设置分频系数
	for(i=0;i<10;i++)//连续读取10次
	{				 
		buf[i]=TPAD_Get_Val();
		delay_ms(10);	    
	}				    
	for(i=0;i<9;i++)//排序
	{
		for(j=i+1;j<10;j++)
		{
			if(buf[i]>buf[j])//升序排列
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}
	temp=0;
	for(i=2;i<8;i++)temp+=buf[i];//取中间的8个数据进行平均
	tpad_default_val=temp/6;
	printf("tpad_default_val:%d\r\n",tpad_default_val);	
	if(tpad_default_val>(vu16)TPAD_ARR_MAX_VAL/2)return 1;//初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常!
	return 0;		     	    					   
}
//复位一次
//释放电容电量，并清除定时器的计数值
void TPAD_Reset(void)
{
    GPIO_InitTypeDef GPIO_Initure;
	
    GPIO_Initure.Pin=GPIO_PIN_1;            //PB1
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;        //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
    
    HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);	//PB1输出0，放电
    delay_ms(5);
    __HAL_TIM_CLEAR_FLAG(&TIM3_Handler,TIM_FLAG_CC4|TIM_FLAG_UPDATE);   //清除标志位
    __HAL_TIM_SET_COUNTER(&TIM3_Handler,0); //计数器值归0
    
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;   //PB1复用为TIM3通道4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);         
}

//得到定时器捕获值
//如果超时,则直接返回定时器的计数值.
//返回值：捕获值/计数值（超时的情况下返回）
u16 TPAD_Get_Val(void)
{
    TPAD_Reset();
    while(__HAL_TIM_GET_FLAG(&TIM3_Handler,TIM_FLAG_CC4)==RESET) //等待捕获上升沿
    {
        if(__HAL_TIM_GET_COUNTER(&TIM3_Handler)>TPAD_ARR_MAX_VAL-500) return __HAL_TIM_GET_COUNTER(&TIM3_Handler);//超时了，直接返回CNT的值
    };
    return HAL_TIM_ReadCapturedValue(&TIM3_Handler,TIM_CHANNEL_4);
}

//读取n次,取最大值
//n：连续获取的次数
//返回值：n次读数里面读到的最大读数值
u16 TPAD_Get_MaxVal(u8 n)
{ 
	u16 temp=0; 
	u16 res=0; 
	u8 lcntnum=n*2/3;//至少2/3*n的有效个触摸,才算有效
	u8 okcnt=0;
	while(n--)
	{
		temp=TPAD_Get_Val();//得到一次值
		if(temp>(tpad_default_val*5/4))okcnt++;//至少大于默认值的5/4才算有效
		if(temp>res)res=temp;
	}
	if(okcnt>=lcntnum)return res;//至少2/3的概率,要大于默认值的5/4才算有效
	else return 0;
}  

//扫描触摸按键
//mode:0,不支持连续触发(按下一次必须松开才能按下一次);1,支持连续触发(可以一直按下)
//返回值:0,没有按下;1,有按下;	
#define TPAD_GATE_VAL 	30	//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
u8 TPAD_Scan(u8 mode)
{
	static u8 keyen=0;	//0,可以开始检测;>0,还不能开始检测	 
	u8 res=0;
	u8 sample=3;		//默认采样次数为3次	 
	u16 rval;
	if(mode)
	{
		sample=6;		//支持连按的时候，设置采样次数为6次
		keyen=0;		//支持连按	  
	}
	rval=TPAD_Get_MaxVal(sample); 
	if(rval>(tpad_default_val+TPAD_GATE_VAL))//大于tpad_default_val+TPAD_GATE_VAL,有效
	{							 
		if(keyen==0)res=1;	//keyen==0,有效 
		//printf("r:%d\r\n",rval);		     	    					   
		keyen=3;				//至少要再过3次之后才能按键有效   
	} 
	if(keyen)keyen--;		   							   		     	    					   
	return res;
}	

//定时器3通道4输入捕获配置
//arr：自动重装值(TIM2是16位的!!)
//psc：时钟预分频数
void TIM3_CH4_Cap_Init(u32 arr,u16 psc)
{  
    TIM_IC_InitTypeDef TIM3_CH4Config;  
    
    TIM3_Handler.Instance=TIM3;                          //通用定时器3
    TIM3_Handler.Init.Prescaler=psc;                     //分频系数
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    TIM3_Handler.Init.Period=arr;                        //自动装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//分频因子
    HAL_TIM_IC_Init(&TIM3_Handler);
    
    TIM3_CH4Config.ICPolarity=TIM_ICPOLARITY_RISING;    //上升沿捕获
    TIM3_CH4Config.ICSelection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM3_CH4Config.ICPrescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM3_CH4Config.ICFilter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_IC_ConfigChannel(&TIM3_Handler,&TIM3_CH4Config,TIM_CHANNEL_4);//配置TIM3通道4
    HAL_TIM_IC_Start(&TIM3_Handler,TIM_CHANNEL_4);      //开始捕获TIM3的通道4
}

//定时器3底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_IC_Init()调用
//htim:定时器3句柄
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM3_CLK_ENABLE();            //使能TIM3时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();			//开启GPIOB时钟
	
    GPIO_Initure.Pin=GPIO_PIN_1;            //PB1
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	 //推挽复用
    GPIO_Initure.Pull=GPIO_NOPULL;          //不带上下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;   //PB1复用为TIM3通道4
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
}
