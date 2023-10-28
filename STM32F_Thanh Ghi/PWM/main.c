#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "main.h"
int main(void)
{
	GPIO_Config();
	Uart_Init();
	Timer2_StandardLibrary();
	//uint16_t duty=0;
	//Send_value_UART(duty+1);
	pwm(10);
	while (1)
	{
		Send_value_UART_2(69);
		Delay_Timer2_StandardLibrary_Ms(10);
		/*if (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==0)
		{
			duty+=5;
			if (duty>100)
			{
				duty=0;
			}
			pwm(duty);
		}*/
	}
}


