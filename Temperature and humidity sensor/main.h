#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stdio.h"
#include "string.h"

void GPIO_Config(void);
void Timer2_StandardLibrary(void);
void Delay_Timer2_StandardLibrary_Ms(uint32_t u32Delay);
void Uart_Init(void);
_Bool UART_Write(USART_TypeDef *USARTx, char *str);
void send_number(uint8_t number);
void tempandhumid(void);
void Send_data_measure(uint8_t u8Buff[5]);

void GPIO_Config(void)
{ 
  GPIO_InitTypeDef  GPIO_InitStructure;  
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/*Enable led PC13 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	/* Enable PB12 (data of sensor temp & humidity) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
		
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
_Bool UART_Write(USART_TypeDef *USARTx, char *str)
{
	 /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(str)); 
	
	/* Transmit Data */
	uint8_t i=0;
	if (strlen(str)<1) return 0;
	for (i=0;i < strlen(str);i++)
	{
		USART_SendData(USARTx,str[i]);
		Delay_Timer2_StandardLibrary_Ms(2);
	}
	return 1;
}
void send_number(uint8_t number)
{
	char str[5]="";
	uint16_t d=0;
	uint8_t kq=1;
	if (number==0)
	{
		number+=48;
	}
	else {
		while (kq>0)
		{
			d+=1;
			kq=number/pow(10,d);
		}
		while (d>0)
		{
			str[d-1]=(char) number%10 +48;
			number=number/10;
			d--;
		}
	}
	UART_Write(USART2,str);
}
void tempandhumid(void)
{
	uint16_t u16time;
	uint8_t u8Buff[5];
	uint8_t u8CheckSum;
	uint8_t i;
	uint8_t d=0;
	//char Buff[40];
	
	/*MCU sends out start signal (at least 18 ms to let DHT11 detect the signal) */
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	Delay_Timer2_StandardLibrary_Ms(20);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
	
	/* Wait until PB12 get value 1 */
	TIM_SetCounter(TIM2,0);
	while (TIM_GetCounter(TIM2)<10)
	{
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
		{
			break;
		}
	}
	u16time=TIM_GetCounter(TIM2);
	if (u16time>=10)
	{
		while (1)
		{}
	}
	
	/* Wait for DHT response (from 20-40 ms) */
	TIM_SetCounter(TIM2,0);
	while (TIM_GetCounter(TIM2)<45)
	{
		if (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
		{
			break;
		}
	}
	u16time=TIM_GetCounter(TIM2);
	if ((u16time<=5) || (u16time>=45))
	{
		while (1)
		{}
	}
	
	/* DHT sends out response signal and keeps it at least 80us (WAIT UNTIL PB12 GET HIGH) */
	TIM_SetCounter(TIM2,0);
	while (TIM_GetCounter(TIM2)<90)
	{
		if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
		{
			break;
		}
	}
	u16time=TIM_GetCounter(TIM2);
	if ((u16time<=70) || (u16time>=90))
	{
		while (1)
		{}
	}
	
	/* DHT pulls up votage and keeps it for 80us (WAIT UNTIL PB12 GET LOW) */
	TIM_SetCounter(TIM2,0);
	while (TIM_GetCounter(TIM2)<95)
	{
		if (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
		{
			break;
		}
	}
	u16time=TIM_GetCounter(TIM2);
	if ((u16time<=75) || (u16time>=95))
	{
		while (1)
		{}
	}
	
/*********************************************************SEND DATA*************************************************************/
	/* DHT11 will send 5 bytes data, which is in order of:
		1 - Byte value of humidity
		2 - decimal part of humidity's value 
		3 - Byte value of temperature
		4 - decimal part of temperature's value
		5 - Byte Parity which is used to check tha data
	*/	
/*-----------------------------------------------------------------------------------------------------------------*/
	
	/*Recieve 5 byte*/
	while (d<5)
	{
		u8Buff[d]=0;
		for (i=0;i<8;i++)
		{
			/* Wait for PB12 get high */
			TIM_SetCounter(TIM2,0);
			while (TIM_GetCounter(TIM2)<65)
			{
				if (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
				{
					break;
				}
			}
			u16time=TIM_GetCounter(TIM2);
			if ((u16time<=45) || (u16time>=65))
			{
				while (1)
				{}
			}
			
			/* Wait for PB12 get LOW */
			TIM_SetCounter(TIM2,0);
			while (TIM_GetCounter(TIM2)<80)
			{
				if (!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))
				{
					break;
				}
			}
			u16time=TIM_GetCounter(TIM2);
			if ((u16time<=10) || (u16time>=80))
			{
				while (1)
				{}
			}
			
	/*---------------------------------------------------*/
			u8Buff[d]<<=1;
			/*check that value = 0 or = 1*/
			if (u16time>45){
				/*nhan duoc bit 1*/
				//Buff[i+8*d]='1';
				u8Buff[d]|=1;
			}
			else {
				/*nhan duoc bit 0*/
				u8Buff[d] &= ~1;
				//Buff[i+8*d]='0';
			}
	/*---------------------------------------------------*/
			/*if (u16time>45){
				Buff[i+8*d]='1';
				u8Buff[d]+=2^(7-i);
			}
			else {
				Buff[i+8*d]='0';
			}
			*/
		}
		d++;
	}
	
	/*Kiem tra xem co dung du lieu hay khong bang cach so sanh byte parity voi 4 byte dau*/
	u8CheckSum=u8Buff[0]+u8Buff[1]+u8Buff[2]+u8Buff[3];
	if (u8CheckSum!=u8Buff[4]){
		while(1)
		{}
	}
	//USART_SendData(USART2,u8Buff[2]);
	//Send_data_measure(u8Buff);
	//send_number((uint8_t)u8Buff[0]);
	Send_data_measure(u8Buff);
	Delay_Timer2_StandardLibrary_Ms(500);
}

/*
hientai
00110011 00000000 00011100 00000101 01010100
00110011 00000000 00011011 00000101 01010011
sau do
*/


/*send data of humidity and temperature through USART*/
void Send_data_measure(uint8_t u8Buff[5])
{
	/*HUMIDITY*/
	UART_Write(USART2,"HUMIDITY:\t");
	Delay_Timer2_StandardLibrary_Ms(2);
	send_number((uint8_t)u8Buff[0]);
	Delay_Timer2_StandardLibrary_Ms(2);
	UART_Write(USART2,"%\n");
	Delay_Timer2_StandardLibrary_Ms(2);
	
	/*TEMPERATURE*/
	UART_Write(USART2,"TEMPERATURE:\t");
	Delay_Timer2_StandardLibrary_Ms(2);
	send_number((uint8_t)u8Buff[2]);
	Delay_Timer2_StandardLibrary_Ms(2);
	UART_Write(USART2," DO C\n");
	Delay_Timer2_StandardLibrary_Ms(2);
}
