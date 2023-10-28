#include "main.h"
#include "stdio.h"
#include "Timer.h"
//void RTC_IRQHandler(void);
//static uint32_t count=0;
int main(){
//setup
	GPIO_Config();
	//USART_Pin_Config();
	//USART_Init_Config(9600);
	//PWM_Standard();
	//PWM_Timer2_Multi_Channels(20,40,60,80);
	Timer2_Register_Interrupt();
	//Timer2_Register();
	//RTC_Config();
	//Interrupt_GPIO_Config();
	//EXIT0();
	//EXTI1_Init();
	//Interrupt_Init_Libra();
	uint32_t x;
//operation
	//char* str1="ASD";
	//char* str;
	while (1)
	{
		 x = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
		//USART_SendData(USART2,USART_ReceiveData(USART2));
		//USART_Write(USART2,"Hello world");
		//str=USART_Receive(USART2);
		//USART_Write(USART2,str);
		
//		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
//		Delay_Timer2_Ms(1000);
//		GPIO_SetBits(GPIOC,GPIO_Pin_13);
//		Delay_Timer2_Ms(1000);
	}		
	
}


void GPIO_Config(void)
{
	/*Led C13 configuration */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_Led;
	GPIO_Led.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Led.GPIO_Pin=GPIO_Pin_13;
	GPIO_Led.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_Led);
	
	/* configuration of PA0 for PWM */
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	
//	GPIO_InitTypeDef GPIO_PWM;
//	GPIO_PWM.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_PWM.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
//	GPIO_PWM.GPIO_Speed=GPIO_Speed_50MHz;
//	
//	GPIO_Init(GPIOA,&GPIO_PWM);
}

