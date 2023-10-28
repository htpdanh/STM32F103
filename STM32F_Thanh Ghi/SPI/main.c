#include "main.h"
#include "stm32f10x.h"
#include "SPI_practice.h"

/*
*PB15 - SPI2 MOSI
*PB14 - SPI2 MISO
*PB13 - SPI2 SCLK
*PB12 - SPI2 NSS
*/
int main(void)
{
	led_init();
	Tim_Config();
	SPI_Config();
	
	//SPI_Send_byte("a");
	//SPI_Cmd(SPI2,DISABLE);
	//char user_data[] = "Hello world";
	//SPI_SendData(SPI2,(uint8_t*)user_data,strlen(user_data));
	while (1)
	{
		//GPIO_ResetBits(GPIOB,GPIO_Pin_12);
		/* Truyen 1 byte */
		//SPI_I2S_SendData(SPI2,0xAD);
		//while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY) == SET);
		/* Truyen nhieu byte*/
		SPI_Send_byte("abc");
		//GPIO_SetBits(GPIOB,GPIO_Pin_12);
		
		/*GPIO_SetBits(GPIOC,GPIO_Pin_13);
		delay(500);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		delay(500);*/
	}
}