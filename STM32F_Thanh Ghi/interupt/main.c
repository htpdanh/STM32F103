#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void GPIO_Config(void);
void EXTI0_StandardLibrary(void);
void EXIT0(void);
void EXTI0_IRQHandler(void);
void Delay(uint32_t nCount) ;

int main(void)
{
	GPIO_Config();
	EXTI0_StandardLibrary();
	while (1)
	{
		/*
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(1000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(1000);
		*/
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	}
}

void GPIO_Config(void)
{ 
   GPIO_InitTypeDef  GPIO_InitStructure; 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB, ENABLE);
	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB,&GPIO_InitStructure);
}
void EXIT0(void){
	/*Enable clock for AFIOEN*/
	RCC->APB2ENR|=0x01;
	AFIO->EXTICR[0]|=0x01;
	/*setup Interupts*/
	EXTI->PR=0x01;
	EXTI->RTSR=0x00;
	EXTI->FTSR|=0x01;
	EXTI->IMR|=0x01;
/**Enable IRQ channels**/
	/*6 settable EXTI0 Line0 interrupt*/
	NVIC->ISER[0] = 0x40;
	
}
void EXTI0_IRQHandler(void){
	/*
	if ((EXTI->PR==1U) && (EXTI->IMR==1U))
	{
		EXTI->PR=0x01;
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(1000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(1000);
	}
	*/
	/*with standard library*/
	if (EXTI_GetITStatus(EXTI_Line0)!=RESET){
		EXTI_ClearITPendingBit(EXTI_Line0);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay(1000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay(1000);
	}
}
void EXTI0_StandardLibrary(void)
{
		EXTI_InitTypeDef EXTI_InitStructure; 
    NVIC_InitTypeDef NVIC_InitStructure;
	
		/* Enable clock for AFIOEN*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		/* External interrupt configuration register 1 (AFIO_EXTICR1) */
	  /* 0001: PB[x] pin */
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
		/* Clear the the EXTI line interrupt pending bit */
		EXTI_ClearITPendingBit(EXTI_Line0);
		/*EXTI line Configuration */
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Line = EXTI_Line0;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
		/*NVIC Configuration*/
		NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}

void Delay(uint32_t nCount) 
{
	while (nCount){
		/*delay 1ms*/
		SysTick->LOAD=72U*1000U-1U;
		SysTick->VAL=0U;//currently value
		SysTick->CTRL=5U;
		while (!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG)){
		}
		nCount--;
	}
}
