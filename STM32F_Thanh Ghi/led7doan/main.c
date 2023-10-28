#include "stm32f10x.h"
void GPIO_config(void);
void led7s_config(uint16_t led); 

int main (void)
{
	//uint16_t count = 0;
	GPIO_config();
	while(1)
	{
		led7s_config(1);
	}
}

void GPIO_config(void)
{
	GPIO_InitTypeDef GPIO_structure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_structure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_structure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_structure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	
	GPIO_Init(GPIOA, &GPIO_structure);
}

static uint8_t LedCode7s[10]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};

void led7s_config(uint16_t led)
{
	uint32_t ledcode = LedCode7s[led];
	//led vtri A
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	}
	ledcode = ledcode >> 1;
	//led vitri B
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
	ledcode = ledcode >> 1;
	//led vitri C
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	}
	ledcode = ledcode >> 1;
	//led vitri D
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	}
	ledcode = ledcode >> 1;
	//led vitri E
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_4);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	}
	ledcode = ledcode >> 1;
	//led vitri F
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	}
	ledcode = ledcode >> 1;
	//led vitri G
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	}
	ledcode = ledcode >> 1;
	//led vitri DP
	if ((ledcode&0x01) == 0x01)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_7);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	}
	ledcode = ledcode >> 1;
}
