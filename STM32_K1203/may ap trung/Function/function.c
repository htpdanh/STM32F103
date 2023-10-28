
#include "variable.h"
#include "main.h"

DS3231_Name DS3231;
DHT_Name DHT;
CLCD_I2C_Name LCD1602;
IR_Name IR;
FAN_Name FAN;
HEATER_Name HEATER;


MACHINE_MODE mode; //0 run, setting

SETTING_POINTER poiter_setting = 0;
RUN_POINTER poiter_run = 0;
TEST_POINTER poiter_test = 0;
uint8_t temp;
uint8_t temp_setting;

uint8_t humi;
uint8_t humi_setting;

uint8_t time_rtc;
uint8_t date_rtc;

uint8_t time_rotate_setting;
uint8_t time_complete_setting;

void LED7_Test(void)
{	
	LED7SEG_Scan4Led(1111);
	HAL_Delay(500);
	LED7SEG_Scan4Led(2222);
	HAL_Delay(500);
	LED7SEG_Scan4Led(3333);
	HAL_Delay(500);
}	

void BUTTON_Test(void)
{
	
	
}	

//App Funct
void hardware_init(void)
{
	
}	

void button_scan(void)
{
	
}	

void button_handle(void)
{
	switch(bt_press)
	{	
		case IDLE:
		{
			//no action
		}	
		break;
		case BT_MODE:
		{
			if(mode == RUN)
			{
				mode = SETTING;
			}				
			else if(mode == SETTING)
			{
				poiter_setting++;
				if(poiter_setting > TIME_COMPL_SETTING) poiter_setting = 0;
			}
			bt_press = IDLE;			
		}			
		break;
		case BT_UP:
		{
			if(poiter_setting == TEMP_SETTING)
			{
				temp_setting++;
			}
			else if(poiter_setting == HUMI_SETTING)
			{
				humi_setting++;
			}
			else if(poiter_setting == TIME_ROTATE_SETTING)
			{
				time_rotate_setting++;
			}
			else if(poiter_setting == TIME_COMPL_SETTING)
			{
				time_complete_setting++;
			}
			
			if(mode == TEST)
			{
				poiter_test++;
			}	
			bt_press = IDLE;		
		}			
		break;
		case BT_DOWN:
		{
			if(poiter_setting == TEMP_SETTING)
			{
				temp_setting--;
			}
			else if(poiter_setting == HUMI_SETTING)
			{
				humi_setting--;
			}
			else if(poiter_setting == TIME_ROTATE_SETTING)
			{
				time_rotate_setting--;
			}
			else if(poiter_setting == TIME_COMPL_SETTING)
			{
				time_complete_setting--;
			}
			bt_press = IDLE;		
		}			
		break;
		case BT_ENTER:
		{
			if(mode == SETTING)
			{
				mode = RUN;
				save_setting();
			}
			bt_press = IDLE;		
		}			
		break;
	}
	#ifdef DEBUG
		printf("button handle done");
	#endif
}	

void sensor_read(void)
{
	DHT_ReadTempHum(&DHT);
	//temp = DHT.Temp;
	//humi = DHT.Humi;
	DS3231_GetTime(&DS3231);
	DS3231_GetDate(&DS3231);
	#ifdef DEBUG
		printf("sensor read done");
	#endif
}

void save_setting(void)
{
	
	
	#ifdef DEBUG
		printf("save setting done");
	#endif
}	
void sensor_handle(void)
{
	if(temp > temp_setting)
	{
		FAN_PWM(&FAN, 100);
	}		
}

void led7_display(void)
{
	if(mode == RUN)
	{
		if(poiter_run == TEMP)
		{
			LED7SEG_Scan4Led(temp);
		}	
		else if(poiter_run == HUMI)
		{
			LED7SEG_Scan4Led(humi);
		}
		else if(poiter_run == TIME)
		{
			LED7SEG_Scan4Led(time_rtc);
		}
		else if(poiter_run == DATE)
		{
			LED7SEG_Scan4Led(date_rtc);
		}
	}	
	else if(mode == SETTING)
	{
		if(poiter_setting == TEMP_SETTING)
		{
			LED7SEG_Scan4Led(temp_setting);
		}
		else if(poiter_setting == HUMI_SETTING)
		{
			LED7SEG_Scan4Led(humi_setting);
		}		
		else if(poiter_setting == TIME_ROTATE_SETTING)
		{
			LED7SEG_Scan4Led(time_rotate_setting);
		}	
		else if(poiter_setting == TIME_COMPL_SETTING)
		{
			LED7SEG_Scan4Led(time_complete_setting);
		}	
	}	
}
