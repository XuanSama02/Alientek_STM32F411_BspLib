#include "bsp_oled.h"
#include "bsp_oled_codetab.h"

I2C_HandleTypeDef OLED_I2C;

/**
 * @brief 初始化OLED屏幕
 * 
 * @param SCL_PORT I2C_SCL端口
 * @param SDA_PORT I2C_SDA端口
 * @param SCL_PIN I2C_SCL引脚
 * @param SDA_PIN I2C_SDA引脚
 */
void OLED_Init(GPIO_TypeDef* SCL_PORT, uint16_t SCL_PIN, GPIO_TypeDef* SDA_PORT, uint16_t SDA_PIN)
{
    //初始化I2C接口
    OLED_I2C.I2C_SCL_PORT = SCL_PORT;
    OLED_I2C.I2C_SCL_PIN = SCL_PIN;
    OLED_I2C.I2C_SDA_PORT = SDA_PORT;
    OLED_I2C.I2C_SDA_PIN = SDA_PIN;
    I2C_Init(&OLED_I2C);
    //初始化OLED屏幕
    osDelay(200);
	OLED_Command(0xAE); //display off
	OLED_Command(0x20);	//Set Memory Addressing Mode	
	OLED_Command(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_Command(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_Command(0xc8);	//Set COM Output Scan Direction
	OLED_Command(0x00); //---set low column address
	OLED_Command(0x10); //---set high column address
	OLED_Command(0x40); //--set start line address
	OLED_Command(0x81); //--set contrast control register
	OLED_Command(0xff); //亮度调节 0x00~0xff
	OLED_Command(0xa1); //--set segment re-map 0 to 127
	OLED_Command(0xa6); //--set normal display
	OLED_Command(0xa8); //--set multiplex ratio(1 to 64)
	OLED_Command(0x3F); //
	OLED_Command(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_Command(0xd3); //-set display offset
	OLED_Command(0x00); //-not offset
	OLED_Command(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_Command(0xf0); //--set divide ratio
	OLED_Command(0xd9); //--set pre-charge period
	OLED_Command(0x22); //
	OLED_Command(0xda); //--set com pins hardware configuration
	OLED_Command(0x12);
	OLED_Command(0xdb); //--set vcomh
	OLED_Command(0x20); //0x20,0.77xVcc
	OLED_Command(0x8d); //--set DC-DC enable
	OLED_Command(0x14); //
	OLED_Command(0xaf); //--turn on oled panel
}

/**
 * @brief I2C发送指令
 * 
 * @param Command 指令
 */
void OLED_Command(uint8_t Command)
{
    I2C_Transmit(&OLED_I2C, OLED_Addr_Dev, 0x00, Command);
}

/**
 * @brief I2C发送数据
 * 
 * @param Data 数据
 */
void OLED_Data(uint8_t Data)
{
    I2C_Transmit(&OLED_I2C, OLED_Addr_Dev, 0x40, Data);
}

/**
 * @brief 全屏填充
 * 
 * @param Fill_Data 填充数据 
 */
void OLED_Fill(uint8_t Fill_Data)
{
	uint8_t aCount,bCount;
	for(aCount=0; aCount<8; aCount++)
	{
		OLED_Command(0xb0 + aCount);		//page0-page1
		OLED_Command(0x00);		//low column start address
		OLED_Command(0x10);		//high column start address
		for(bCount=0; bCount<128; bCount++)
			OLED_Data(Fill_Data);
	}
}

/**
 * @brief 设置起始点坐标
 * 
 * @param x X轴坐标
 * @param y Y轴坐标
 */
void OLED_SetPos(uint8_t x, uint8_t y)
{
	OLED_Command(0xb0 + y);
	OLED_Command(((x&0x0F) >> 4) | 0x10);
	OLED_Command((x&0x0F) | 0x01);
}

/**
 * @brief OLED清屏
 * 
 */
void OLED_CLS(void)
{
	OLED_Fill(0x00);
}

/**
 * @brief 开启OLED
 * 
 */
void OLED_ON(void)
{
	OLED_Command(0x8D);  //设置电荷泵
	OLED_Command(0x14);  //开启电荷泵
	OLED_Command(0xAF);  //OLED唤醒
}

/**
 * @brief 关闭OLED
 * 
 */
void OLED_OFF(void)
{
	OLED_Command(0x8D);  //设置电荷泵
	OLED_Command(0x10);  //开启电荷泵
	OLED_Command(0xAE);  //OLED唤醒
}

/**
 * @brief OLED显示字符串
 * 
 * @param x 起始点X轴坐标
 * @param y 起始点Y轴坐标
 * @param ch 要显示的字符串
 * @param TextSize 字体大小(TextSize_Small/TextSize_Big)
 */
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t ch[], bool TextSize)
{
	uint8_t aCount = 0,bCount = 0,cCount = 0;
	switch(TextSize)
	{
		case TextSize_Small:
		{
			while(ch[cCount] != '\0')
			{
				aCount = ch[cCount] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(bCount=0;bCount<6;bCount++)
					OLED_Data(F6x8[aCount][bCount]);
				x += 6;
				cCount++;
			}
		}break;
		case TextSize_Big:
		{
			while(ch[cCount] != '\0')
			{
				aCount = ch[cCount] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetPos(x,y);
				for(bCount=0;bCount<8;bCount++)
					OLED_Data(F8X16[aCount*16+bCount]);
				OLED_SetPos(x,y+1);
				for(bCount=0;bCount<8;bCount++)
					OLED_Data(F8X16[aCount*16+bCount+8]);
				x += 8;
				cCount++;
			}
		}break;
	}
}
