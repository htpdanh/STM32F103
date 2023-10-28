#include "stm32f10x.h"
#include "Interrupt.h"

/*Purpose: Create Interrups at pin PB0 or PB1.
	-ENABLE RCC at Port B
	-Config for PB0 at mode IPU(Input) because we want to create a button
	-Enable RCC for AFIO
	-Enable which is EXTI (AFIO->EXTICR[(1->19)]
	-Config Interrupt for PB0 at EXTI
	-Enable NVIC at ISER[0]
*/

/*Result: Whenever press the button at PB0, led at PC13 will RESET */

void Interrupt_GPIO_Config(void)
{
	//Enable RCC_IOPB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//Enable 
	GPIO_InitTypeDef GPIOx;
	
	GPIOx.GPIO_Mode=GPIO_Mode_IPU;
	GPIOx.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIOx.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIOx);
}

void Interrupt_Init_Libra(void)
{
	EXTI_InitTypeDef EXTIx;
	NVIC_InitTypeDef NVIC_Config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/* External interrupt configuration register 1 (AFIO_EXTICR1) */
	/* 0001: PB[x] pin */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);
	/* Clear the the EXTI line interrupt pending bit */
	EXTI_ClearITPendingBit(EXTI_Line1);
	/*EXTI line Configuration */
	EXTIx.EXTI_Line=EXTI_Line1;
	EXTIx.EXTI_LineCmd=ENABLE;
	EXTIx.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTIx.EXTI_Trigger=EXTI_Trigger_Falling;
	EXTI_Init(&EXTIx);
	
	/*NVIC Configuration*/
	NVIC_Config.NVIC_IRQChannel=EXTI1_IRQn;
	NVIC_Config.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Config.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_Init(&NVIC_Config);
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
	NVIC->ISER[0] = 0x40;	//0100 0000
}

void EXTI1_Init(void)
{
	//Enable RCC_AFIO (Alternate function)
	RCC->APB2ENR|=0x01;
	//Enable AFIO_EXTICR1 (External interrupt configuration register 1) PBx = 0001
	AFIO->EXTICR[0]|=AFIO_EXTICR1_EXTI1_PB;
	//Set chan EXTI_PR de cho chan PR dc reset
	EXTI->PR=0x02;
	//Enable EXTI_IMR chan MR0 (Interrupt mask register)
	EXTI->IMR|=0x02;
	//Enable EXTI_FTSR (Falling trigger selection register)
	EXTI->FTSR|=0x02;	
	/*Enable IRQ Channels*/
	/*6 settable EXTI1 Line1 interrupt*/
	NVIC->ISER[0] = 0x80;	//1000 0000 (EXTI1 nam vi tri thu 7 trong bang NVIC)
}

void EXTI0_IRQHandler(void)
{
	if ((EXTI->PR==1U) && (EXTI->IMR==0x01)){
		EXTI->PR=0x01;
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_Ms(1000);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay_Ms(1000);
	}
}

void EXTI1_IRQHandler(void)
{
	if ((EXTI->PR==0x02) && (EXTI->IMR==0x02)){
		EXTI->PR=0x02;
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_Ms(1000);
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay_Ms(1000);
	}
}


//void Delay(uint32_t nCount) 
//{
//	while (nCount){
//		/*delay 1ms*/
//		SysTick->LOAD=72U*1000U-1U;
//		SysTick->VAL=0U;//currently value
//		SysTick->CTRL=5U;
//		while (!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG)){
//		}
//		nCount--;
//	}
//}
