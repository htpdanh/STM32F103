#include "USART.h"
#include "string.h"

void USART_Pin_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIOx;
	GPIOx.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIOx.GPIO_Pin=GPIO_Pin_2|GPIO_Pin_3;
	GPIOx.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIOx);
}

void USART_Init_Config(uint32_t BaudRate)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	USART_InitTypeDef USARTx;
	USARTx.USART_BaudRate=BaudRate;
	USARTx.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USARTx.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USARTx.USART_Parity=USART_Parity_No;
	USARTx.USART_StopBits=USART_StopBits_1;
	USARTx.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART2,&USARTx);
	
	/* Enable RX Interrupt */
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);
}

void USART_Write(USART_TypeDef *USARTx, char*Str)
{
	uint8_t i = 0;
	for (i=0 ; i<strlen(Str) ; i++)
	{
		USART_SendData(USARTx,Str[i]);
		Delay_Timer2_Ms(1);
	}
}

/*loi receive nhieu phan tu cung 1 luc*/
char* USART_Receive(USART_TypeDef *USARTx)
{
	uint16_t Chat;
	char* str="";
	//while ((USARTx->SR & (1<<5)) == 0x00 );
	Chat=USART_ReceiveData(USARTx);
	*str=*str+(char)Chat;
	return str;
}

void USART_ReceiveIT_Setup(void)
{
	//USART in NVIC = 38
	NVIC->ISER[1]|=1<<7;//ISER[0] (Interrupts 0-31) ISER[1] (32-63) in Mapping of interrupts in coreM3;
	NVIC->ICPR[1]|=1<<7;
}


void USART2_IRQHandler(void)
{
	uint16_t ch=0;
	if ((USART_GetFlagStatus(USART2,USART_FLAG_RXNE)&(1 <<5 )) == 0)
	{
		ch = (uint16_t) USART_ReceiveData(USART2);
	}
	USART_SendData(USART2,ch);
	
	USART_ClearFlag(USART2,USART_FLAG_RXNE);
	NVIC->ICPR[1]|=1<<7;
}
