#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "usmart.h"
#include "spi.h"
#include "norflash.h"

/************************************************
 ALIENTEK NANO STM32F4开发板实验19
 SPI实验-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

//要写入到W25Q16的字符串数组
const u8 TEXT_Buffer[] = {"NANOSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)

int main(void)
{
    u8 key;
    u16 i = 0;
    u16 id=0;
    u8 datatemp[SIZE];
    u32 FLASH_SIZE;

    HAL_Init();                    	//初始化HAL库
    Stm32_Clock_Init(96, 4, 2, 4);  //设置时钟,96Mhz
    delay_init(96);                 //初始化延时函数
    uart_init(115200);				//初始化串口115200
    LED_Init();                     //初始化LED
    KEY_Init();				        //按键初始化
    Norflash_Init();				    //W25QXX初始化
    usmart_dev.init(96);	        //初始化USMART
    printf("NANO STM32\r\n");
    printf("SPI TEST\r\n");

    id = Norflash_ReadID();//读取FLASH ID
    printf("id:%#x\r\n",id); 
    
    while ((id==0) && (id==0XFFFF)) //检测不到FLASH芯片
    {
        printf("Flash Check Failed!\r\n");
        delay_ms(500);
        printf("Please Check!\r\n");
        delay_ms(500);
        LED0 = !LED0; //DS0闪烁
    }

    printf("Spi Flash Ready!\r\n");
    printf("KEY_UP:Write  KEY1:Read\r\n");
    FLASH_SIZE = 2 * 1024 * 1024;	//FLASH 大小为2M字节

    while (1)
    {
        key = KEY_Scan(0);

        if (key == WKUP_PRES)	//WK_UP按下,写入25Q16
        {
            LED2 = 0;
            printf("\r\nStart Write Flash....\r\n");
            Norflash_Write((u8 *)TEXT_Buffer, FLASH_SIZE - 100, SIZE);	//从倒数第100个地址处开始,写入SIZE长度的数据
            printf("Flash Write Finished!\r\n");//提示传送完成
            LED2 = 1;
        }

        if (key == KEY1_PRES)	//KEY1 按下,读取字符串并显示
        {
            LED2 = 0;
            printf("\r\nStart Read Flash....\r\n");
            Norflash_Read(datatemp, FLASH_SIZE - 100, SIZE);	//从倒数第100个地址处开始,读出SIZE个字节
            printf("The Data Readed Is:  \r\n");//提示传送完成
            printf("%s\r\n", datatemp); //显示读到的字符串
            LED2 = 1;
        }

        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0 = !LED0; //提示系统正在运行
            i = 0;
        }

    }
}

