#include "stm32f10x.h"
#include "stdio.h"

void Tim_Config(void);
void delay(uint32_t u32);
void led_init(void);
void SPI_Config(void);
void SPI_Send_byte(char* buffer);
void DMA_Config(uint32_t buffer);

void Tim_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef Tim_config;
	
	Tim_config.TIM_CounterMode=TIM_CounterMode_Up;
	Tim_config.TIM_Period=0xFFFF;
	Tim_config.TIM_Prescaler=72-1;
	
	TIM_TimeBaseInit(TIM2,&Tim_config);
	TIM_Cmd(TIM2,ENABLE);
}

void delay(uint32_t u32)
{
	while (u32>0)
	{
		TIM_SetCounter(TIM2,0);
		while (TIM_GetCounter(TIM2)<1000);
		u32--;
	}
}

void led_init(void)
{
	GPIO_InitTypeDef gpio_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	gpio_config.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_config.GPIO_Pin=GPIO_Pin_13;
	gpio_config.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&gpio_config);
}

void SPI_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	/* Config registers for SPI */
	GPIO_InitTypeDef gpio_config;
	
	/* B12 - NSS */
	gpio_config.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio_config.GPIO_Pin=GPIO_Pin_12;
	gpio_config.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&gpio_config);
	/* B13 - SCK */
	gpio_config.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_config.GPIO_Pin=GPIO_Pin_13;
	gpio_config.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&gpio_config);
	/* B14 - MISO */
	gpio_config.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	gpio_config.GPIO_Pin=GPIO_Pin_14;
	gpio_config.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&gpio_config);
	/* B15 - MOSI */
	gpio_config.GPIO_Mode=GPIO_Mode_AF_PP;
	gpio_config.GPIO_Pin=GPIO_Pin_15;
	gpio_config.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&gpio_config);
	
	/* SPI - CONFIGURATION */
	SPI_InitTypeDef spi2;
	
	spi2.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_32;
	spi2.SPI_CPHA=SPI_CPHA_1Edge;
	spi2.SPI_CPOL=SPI_CPOL_Low;
	spi2.SPI_DataSize=SPI_DataSize_8b;
	spi2.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	spi2.SPI_FirstBit=SPI_FirstBit_MSB;
	spi2.SPI_Mode=SPI_Mode_Master;
	spi2.SPI_NSS=SPI_NSS_Soft;
	
	SPI_Init(SPI2,&spi2);
	
	SPI_Cmd(SPI2,ENABLE);
}
void SPI_Send_byte(char* buffer)
{
	uint16_t i;
	for (i=0;i<strlen(buffer);i++)
	{
		SPI_I2S_SendData(SPI2,buffer[i]);
		while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)==SET);
	}
}

/* Cau hinh DMA
	De lam duoc DMA thi truoc tien phai biet so byte can gui*/

void DMA_Config(uint32_t buffer)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef dma_config;
	dma_config.DMA_BufferSize=8*1024;
	dma_config.DMA_DIR=DMA_DIR_PeripheralDST;//truyen du lieu tu vung nho toi ngoai vi
	dma_config.DMA_M2M=DMA_M2M_Disable;//truyen du lieu giua 2 vung nho
	dma_config.DMA_MemoryBaseAddr = (uint32_t) buffer;//dia chi ma minh muon truyen
	dma_config.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte;
	dma_config.DMA_MemoryInc=DMA_MemoryInc_Enable;//sau khi truyen no se tu dong chuyen sang ki tu khac de truyen tiep
	dma_config.DMA_Mode=DMA_Mode_Normal;
	dma_config.DMA_PeripheralBaseAddr=(uint32_t) &SPI2->DR;
	dma_config.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	dma_config.DMA_Priority=DMA_Priority_Medium;
	
	DMA_Init(DMA1_Channel5,&dma_config);
	DMA_Cmd(DMA1_Channel5,ENABLE);
	SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
}
