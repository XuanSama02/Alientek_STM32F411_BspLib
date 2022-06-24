#include "bsp_spi_lcd.h"

void LCD_Init(void)
{
    //开启时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //配置GPIO引脚
    GPIO_InitTypeDef LCD = {0};
    LCD.Mode = GPIO_MODE_OUTPUT_PP;
    LCD.Pull = GPIO_NOPULL;
    LCD.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    //初始化GPIO引脚
    LCD.Pin = LCD_BLK_PIN;
    HAL_GPIO_Init(LCD_BLK_PORT, &LCD);
    LCD.Pin = LCD_DC_PIN;
    HAL_GPIO_Init(LCD_DC_PORT, &LCD);
    LCD.Pin = LCD_CS_PIN;
    HAL_GPIO_Init(LCD_CS_PORT, &LCD);
    //配置SPI接口
    SPI_HandleTypeDef hspi1;
    hspi1.Instance = SPI2;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
        printf("SPI1 Init Failed!\r\n");
}

//引脚操作函数

void LCD_PIN_Write(uint8_t LCD_PIN, GPIO_PinState Status);
