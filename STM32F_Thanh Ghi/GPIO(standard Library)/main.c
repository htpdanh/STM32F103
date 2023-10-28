#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void GPIO_Config(void);


void Delay(uint32_t nCount) ;

int main(void)
{
	//GPIO_Config();
	RCC->APB2ENR|=RCC_APB2ENR_IOPCEN;
	GPIOC->CRH|=0x00300000;
	while (1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(10000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(10000);
	}
}

void GPIO_Config(void)
{ 
   GPIO_InitTypeDef  GPIO_InitStructure; 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
   GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void Delay(uint32_t nCount) 
{
	for (int i=0;i<1000;i++)
	{
		while(nCount--)
		{}
	}
}
