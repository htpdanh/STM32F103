#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void GPIO_Config(void);
void Timer2_Register(void);
void Delay_Timer2_Ms(uint32_t nCount) ;
void Timer2_StandardLibrary(void);
void Delay_Timer2_StandardLibrary_Ms(uint32_t u32Delay);

int main(void)
{
	GPIO_Config();
	//Timer2_Register();
	Timer2_StandardLibrary();
	
	while (1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		//Delay_Timer2_Ms(100);
		Delay_Timer2_StandardLibrary_Ms(1000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		//Delay_Timer2_Ms(100);
		Delay_Timer2_StandardLibrary_Ms(1000);
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

void Delay_Timer2_Ms(uint32_t nCount) 
{
	while(nCount)
	{
		TIM2->CNT=0U;
		while ((TIM2->CNT)<1000);
		nCount--;
	}
}

void Timer2_Register(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM2->ARR=0xFFFF;
	TIM2->PSC=72-1;
	TIM2->CR1|=0x01;
	TIM2->EGR|=0x01;
}

void Timer2_StandardLibrary(void)
{
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TimerInit.TIM_CounterMode=TIM_CounterMode_Up;
	TimerInit.TIM_Period=0xFFFF;
	TimerInit.TIM_Prescaler=72-1;
	
	TIM_TimeBaseInit(TIM2,&TimerInit);
	
	TIM_Cmd(TIM2,ENABLE);
}

void Delay_Timer2_StandardLibrary_Ms(uint32_t u32Delay)
{
	while(u32Delay)
	{
		TIM_SetCounter(TIM2,0U);
		while ((TIM_GetCounter(TIM2))<1000U);
		u32Delay--;
	}
}
