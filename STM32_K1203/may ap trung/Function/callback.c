#include "main.h"
#include "variable.h"

uint32_t time_display_count=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //1ms
{
	
  if(htim->Instance == TIM3)
	{
		led7_display();
	}		
	time_display_count++;
	if(time_display_count > 5000)
	{
		poiter_run++;
		time_display_count = 0;
		if(poiter_run >3) 
		{
			poiter_run = 0;
		}
	}	
}

uint32_t button_timout =0;
uint32_t pre_button_timout =0;
uint8_t button_count = 0;

BT_STATUS bt_press;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(GPIO_Pin == GPIO_PIN_0)
	{
		bt_press = BT_MODE;
		button_timout = HAL_GetTick();
		if(button_timout - pre_button_timout < 1000)
		{	
			button_count++;
			pre_button_timout = button_timout;
		}
		else button_count = 0;
		if(button_count > 7)
		{
			mode = TEST;
			button_count = 0;
		}	
	}
	if(GPIO_Pin == GPIO_PIN_0)
	{
		bt_press = BT_UP;
	}
	if(GPIO_Pin == GPIO_PIN_0)
	{
		bt_press = BT_DOWN;
	}
	if(GPIO_Pin == GPIO_PIN_0)
	{
		bt_press = BT_ENTER;
	}
}