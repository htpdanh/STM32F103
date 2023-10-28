#include "stm32f10x.h"
#include "stm32f10x_gpio.h"

void I2C_GPIO_Config(void);
void Timer2_StandardLibrary(void);
void Delay_Timer2_StandardLibrary_Ms(uint32_t u32Delay);
void I2C_Init_Config(void);

void I2C_GPIO_Config(void)
{
	//1.Enable Clock
	RCC_APB1PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	//Led C-13
  GPIO_InitTypeDef  GPIO_Led;  
	
  GPIO_Led.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Led.GPIO_Pin = GPIO_Pin_13;
  GPIO_Led.GPIO_Speed = GPIO_Speed_50MHz;
	
  GPIO_Init(GPIOC, &GPIO_Led); 
	
	//SDA-SCL
	GPIO_InitTypeDef  GPIO_SCL_SDA;  
	
	//cau hinh cho chan SDA va SCL
	GPIO_SCL_SDA.GPIO_Mode=GPIO_Mode_AF_OD;
	GPIO_SCL_SDA.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_11;
	GPIO_SCL_SDA.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_SCL_SDA);
}

void I2C_Init_Config(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	I2C_InitTypeDef i2c_str;
	//cau hinh I2C
	i2c_str.I2C_Mode=I2C_Mode_I2C;
	i2c_str.I2C_DutyCycle=I2C_DutyCycle_2;
	i2c_str.I2C_OwnAddress1=0;//do la master nen chon 0
	i2c_str.I2C_Ack=I2C_Ack_Disable;
	i2c_str.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	i2c_str.I2C_ClockSpeed=100000;
	
	I2C_Init(I2C2,&i2c_str);
	I2C_Cmd(I2C2,ENABLE);
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

