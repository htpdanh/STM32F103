#include "main.h"

/*	purpose: SELECT MODE WITH SINGLE BUTTON
- MODE 1: LED blinks with cycle 1s ( Press the button 1 times)
- MODE 2: LED blinks with cycle 200ms (Press the button 3 times)
- MODE 3: the clock will be active to count ( hold the button at least 5 seconds)
*/
static uint32_t CountButton = 0;

int main(void)
{
	GPIO_Config();
	EXTI_0();
	Timer2_Register_Interrupt();
	while (1)
	{
		if (CountButton>0) {
			delay_ms(500);
			switch (CountButton) {
				case 1:
				{
					task_1();
					break;
				}
				case 3:
				{
					task_2();
					break;
				}
				case 5:
				{
					task_3();
					break;
				}
				//default: CountButton=0;
			}
		}
	}
}

void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_Output,GPIO_Input;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//Output
	GPIO_Output.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Output.GPIO_Pin = GPIO_Pin_13;
	GPIO_Output.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_Output);
	
	//Input 
	GPIO_Input.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_Input.GPIO_Pin=GPIO_Pin_0;
	GPIO_Input.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Input);
}

/*LED blinks with cycle 1s*/
void task_1(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(1000);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(1000);
}

/*LED blinks with cycle 200ms*/
void task_2(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	delay_ms(200);
	GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	delay_ms(200);
}

void task_3(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}

void EXTI0_IRQHandler(void)
{
	TIM2->CR1=0x01;//counter enable
	delay_ms(200);
	if ((EXTI->PR==0x01) && (EXTI->IMR==0x01)){
		EXTI->PR=0x01;
		CountButton++;
	}
}

void TIM2_IRQHandler(void)
{
	if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == 0)
	{
		CountButton = 4;
	}
	TIM2->SR &= ~0x01;
	//TIM1->CR1 &=~0x01;
}

