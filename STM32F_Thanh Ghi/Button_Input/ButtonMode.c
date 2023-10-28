#include "ButtonMode.h"

void delay_ms(uint32_t nCount)
{
	while (nCount >0) 
	{
		SysTick->CTRL|=SysTick_CTRL_CLKSOURCE;
		SysTick->CTRL|=SysTick_CTRL_ENABLE;
		SysTick->LOAD = 72*1000-1U;
		SysTick->VAL = 0U;
		while (!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG));
		nCount--;
	}
}

void EXTI_0(void){
	/*Enable clock for AFIOEN*/
	RCC->APB2ENR|=0x01;
	AFIO->EXTICR[0]|=0x00; //GPIO_A -> 0000: PA[x] pin
	/*setup Interupts*/
	EXTI->PR=0x01;
	EXTI->RTSR=0x01;
	EXTI->FTSR|=0x00;
	EXTI->IMR|=0x01;
/**Enable IRQ channels**/
	/*6 settable EXTI0 Line0 interrupt*/
	NVIC->ISER[0] = 0x40;	//0100 0000
}

void Timer2_Register_Interrupt(void)
{
	/* Enable clock for timer 2*/
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
	/* PSC[15:0]: Prescaler value*/
	TIM2->PSC=7200U-1U;	//	72/(72-1+1)=1MHz
	/* IMx auto-reload register (TIMx_ARR) */
	TIM2->ARR=50000-1;
	TIM2->CNT=0U;
	/* Bit 0 CEN: Counter enable and 0: Counter used as upcounter*/
	//TIM2->CR1=0x01;//counter enable
	/*Generate an update event to reload the Prescaler and the Repetition counter values immediately*/
	TIM2->EGR=0x01;
	/* Bit 0 UIE: Update interrupt enable*/
	TIM2->DIER|=TIM_DIER_UIE;
	/* Clear interrupt flag */
	TIM2->SR&= ~(0x01);
	/* 28 settable TIM2 interrupt*/
	NVIC->ISER[0]=0x10000000;
}
