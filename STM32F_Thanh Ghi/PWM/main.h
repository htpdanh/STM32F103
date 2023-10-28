#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdio.h"
#include "string.h"


void GPIO_Config(void);
void Timer2_StandardLibrary(void);
void Delay_Timer2_StandardLibrary_Ms(uint32_t u32Delay);
void pwm(uint16_t duty);
void Uart_Init(void);
void Send_value_UART(uint16_t u32);
void Send_value_UART_2(uint16_t u32);
_Bool UART_Write(USART_TypeDef *USARTx, char *str);

void GPIO_Config(void)
{ 
	// LED at C13
  GPIO_InitTypeDef  GPIO_InitStructure;  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	//PWM
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//Button at C15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void Timer2_StandardLibrary(void)
{
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TimerInit.TIM_CounterMode=TIM_CounterMode_Up;
	/* dieu chinh so giay trong 1 chu ki cua timmer
	voi gia tri = 10000 thi T=1s*/
	TimerInit.TIM_Period=10000-1;
	TimerInit.TIM_Prescaler=7200-1;
	
	TIM_TimeBaseInit(TIM2,&TimerInit);
	
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_OCInitTypeDef pwmInit;
	/* Ch2 with duty 10% */
	/* PWMode 1 ung voi 10% se la muc cao, PWmod 2 thi nguoc lai */
	pwmInit.TIM_OCMode=TIM_OCMode_PWM1;
	pwmInit.TIM_OCNPolarity=TIM_OCPolarity_High;//quyet dinh TIM_pulse la xung cao
	pwmInit.TIM_Pulse=(10*10000)/100; //phan tram giua xung cao hay xung thap
	pwmInit.TIM_OutputState=TIM_OutputState_Enable;//chon pwm output 
	TIM_OC2Init(TIM2,&pwmInit);
	
//	/* Ch3 with duty 25% */
//	pwmInit.TIM_OCMode=TIM_OCMode_PWM1;
//	pwmInit.TIM_OCNPolarity=TIM_OCPolarity_High;//quyet dinh TIM_pulse la xung cao
//	pwmInit.TIM_Pulse=(25*100)/100; //phan tram giua xung cao hay xung thap
//	pwmInit.TIM_OutputState=TIM_OutputState_Enable;//chon pwm output 
//	TIM_OC3Init(TIM2,&pwmInit);
//	
//	/* Ch4 with duty 50% */
//	pwmInit.TIM_OCMode=TIM_OCMode_PWM1;
//	pwmInit.TIM_OCNPolarity=TIM_OCPolarity_High;//quyet dinh TIM_pulse la xung cao
//	pwmInit.TIM_Pulse=(50*100)/100; //phan tram giua xung cao hay xung thap
//	pwmInit.TIM_OutputState=TIM_OutputState_Enable;//chon pwm output 
//	TIM_OC4Init(TIM2,&pwmInit);
}

void pwm(uint16_t duty)
{
	TIM_OCInitTypeDef pwmInit;
	/* Ch2 with duty 10% */
	/* PWMode 1 ung voi 10% se la muc cao, PWmod 2 thi nguoc lai */
	pwmInit.TIM_OCMode=TIM_OCMode_PWM2;
	pwmInit.TIM_OCNPolarity=TIM_OCPolarity_High;//quyet dinh TIM_pulse la xung cao
	pwmInit.TIM_Pulse=(duty*10000)/100; //phan tram giua xung cao hay xung thap
	pwmInit.TIM_OutputState=TIM_OutputState_Enable;//chon pwm output 
	TIM_OC2Init(TIM2,&pwmInit);
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

void Uart_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN,ENABLE);
	
	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	USART_InitTypeDef USART_Config;
	USART_Config.USART_BaudRate=9600;
	USART_Config.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_Config.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_Config.USART_Parity=USART_Parity_No;
	USART_Config.USART_WordLength=USART_WordLength_8b;
	
	USART_Init(USART2,&USART_Config);
	
	USART_Cmd(USART2,ENABLE);
}

/* chuyen kieu du lieu uint16_t voi so hon 2 chu so sang kieu chuoi va gui sang uart*/
void Send_value_UART(uint16_t u32)
{
	//uint16_t n=3;
	char str[5]="";
	char c;
	uint16_t i=0;
	while (u32>0)
	{
		//Delay_Timer2_StandardLibrary_Ms(5);
		c=(char)(u32%10+48);
		u32=(uint16_t) u32/10;
		str[i]=c;
		i++;
	}
	UART_Write(USART2,str);	
	//return str;
}

void Send_value_UART_2(uint16_t u32)
{
	//uint16_t n=3;
	uint16_t j;
	for (j=0;j<=10;j++)
	{
		if (u32%(10^j)<10){
			break;
		}
	}
	char* str= (char*) calloc(j+1,sizeof(char));
	char c;
	uint16_t i=0;
	while (u32>0)
	{
		//Delay_Timer2_StandardLibrary_Ms(5);
		c=(char)(u32%10+48);
		u32=(uint16_t) u32/10;
		str[i]=c;
		i++;
	}
	UART_Write(USART2,str);	
	//return str;
}

_Bool UART_Write(USART_TypeDef *USARTx, char str[])
{
	 /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(str)); 
	
	/* Transmit Data */
	int8_t i=0;
	if (strlen(str)<1) return 0;
	for (i= strlen(str);i >= 0;i--)
	{
		USART_SendData(USARTx,str[i]);
		Delay_Timer2_StandardLibrary_Ms(1);
	}
	return 1;
}
