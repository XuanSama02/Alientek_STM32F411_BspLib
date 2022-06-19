#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "usmart.h"
#include "spi.h"
#include "norflash.h"

/************************************************
 ALIENTEK NANO STM32F4������ʵ��19
 SPIʵ��-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//Ҫд�뵽W25Q16���ַ�������
const u8 TEXT_Buffer[] = {"NANOSTM32 SPI TEST"};
#define SIZE sizeof(TEXT_Buffer)

int main(void)
{
    u8 key;
    u16 i = 0;
    u16 id=0;
    u8 datatemp[SIZE];
    u32 FLASH_SIZE;

    HAL_Init();                    	//��ʼ��HAL��
    Stm32_Clock_Init(96, 4, 2, 4);  //����ʱ��,96Mhz
    delay_init(96);                 //��ʼ����ʱ����
    uart_init(115200);				//��ʼ������115200
    LED_Init();                     //��ʼ��LED
    KEY_Init();				        //������ʼ��
    Norflash_Init();				    //W25QXX��ʼ��
    usmart_dev.init(96);	        //��ʼ��USMART
    printf("NANO STM32\r\n");
    printf("SPI TEST\r\n");

    id = Norflash_ReadID();//��ȡFLASH ID
    printf("id:%#x\r\n",id); 
    
    while ((id==0) && (id==0XFFFF)) //��ⲻ��FLASHоƬ
    {
        printf("Flash Check Failed!\r\n");
        delay_ms(500);
        printf("Please Check!\r\n");
        delay_ms(500);
        LED0 = !LED0; //DS0��˸
    }

    printf("Spi Flash Ready!\r\n");
    printf("KEY_UP:Write  KEY1:Read\r\n");
    FLASH_SIZE = 2 * 1024 * 1024;	//FLASH ��СΪ2M�ֽ�

    while (1)
    {
        key = KEY_Scan(0);

        if (key == WKUP_PRES)	//WK_UP����,д��25Q16
        {
            LED2 = 0;
            printf("\r\nStart Write Flash....\r\n");
            Norflash_Write((u8 *)TEXT_Buffer, FLASH_SIZE - 100, SIZE);	//�ӵ�����100����ַ����ʼ,д��SIZE���ȵ�����
            printf("Flash Write Finished!\r\n");//��ʾ�������
            LED2 = 1;
        }

        if (key == KEY1_PRES)	//KEY1 ����,��ȡ�ַ�������ʾ
        {
            LED2 = 0;
            printf("\r\nStart Read Flash....\r\n");
            Norflash_Read(datatemp, FLASH_SIZE - 100, SIZE);	//�ӵ�����100����ַ����ʼ,����SIZE���ֽ�
            printf("The Data Readed Is:  \r\n");//��ʾ�������
            printf("%s\r\n", datatemp); //��ʾ�������ַ���
            LED2 = 1;
        }

        i++;
        delay_ms(10);

        if (i == 20)
        {
            LED0 = !LED0; //��ʾϵͳ��������
            i = 0;
        }

    }
}

