#include "DS18B20.h"

extern TIM_HandleTypeDef htim2;
/*----------------------------------------------------LOW-LEVEL LAYER------------------------------------------*/
void Delay_us(uint32_t us)
{
	__HAL_TIM_SET_COUNTER(&htim2,0);
	while((__HAL_TIM_GetCounter(&htim2)<us));
}

void DS18B20_Init(DS18B20_NAME* DS18B20,GPIO_TypeDef* DS18B20_PORT,uint16_t DS18B20_PIN)
{
	DS18B20->Port = DS18B20_PORT;
	DS18B20->Pin = DS18B20_PIN;
}

void DS18B20_WritePin(DS18B20_NAME* DS18B20,uint8_t PinState){
	HAL_GPIO_WritePin(DS18B20->Port,DS18B20->Pin,PinState);
}

uint8_t DS18B20_ReadPin(DS18B20_NAME* DS18B20)
{
	return HAL_GPIO_ReadPin(DS18B20->Port,DS18B20->Pin);
}

void Set_Pin_Output(DS18B20_NAME* DS18B20)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS18B20->Port, &GPIO_InitStruct);
}

void Set_Pin_Input(DS18B20_NAME* DS18B20)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20->Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DS18B20->Port, &GPIO_InitStruct);
}

/*----------------------------------------------------MIDDLE LEVEL LAYER------------------------------------------*/
uint8_t DS18B20_Reset(DS18B20_NAME* DS18B20)
{
	uint8_t State;
	/*the bus master transmits (TX) the reset pulse by pulling the 1-Wire bus low for a minimum of 480탎*/
	Set_Pin_Output(DS18B20);
	DS18B20_WritePin(DS18B20,0);
	Delay_us(480);
	
	/* When the DS18B20 detects this rising edge, it waits 15탎 to 60탎 and then transmits a presence pulse by pulling the 1-Wire bus low for 60탎 to 240탎*/
	Set_Pin_Input(DS18B20);
	Delay_us(60);
	
	if (!DS18B20_ReadPin(DS18B20)) State=1;
	else State=0;
	Delay_us(420);
	
	return State;
}

void DS18b20_WriteData(DS18B20_NAME* DS18B20,uint8_t Data)
{
	Set_Pin_Output(DS18B20);
	/*. All write time slots must be a minimum of 60탎 in duration with a minimum of a 1탎 recovery time between individual write slots*/
	for (int i=0; i<8; i++){
		if ((Data&(1<<i))==0) {
			//Write 0
			/*To generate a Write 0 time slot, after pulling the 1-Wire bus low (at least 60탎)*/
			Set_Pin_Output(DS18B20);
			DS18B20_WritePin(DS18B20,0);
			Delay_us(60);
			DS18B20_WritePin(DS18B20,1);
		}
		else {
			//Write 1
			/*To generate a Write 1 time slot, after pulling the 1-Wire bus low, the bus master must release the 1-Wire bus within 15탎. */
			Set_Pin_Output(DS18B20);
			DS18B20_WritePin(DS18B20,0);
			Delay_us(15);
			DS18B20_WritePin(DS18B20,1);
			Delay_us(45);
		}
	}
}
uint8_t DS18b20_ReadData(DS18B20_NAME* DS18B20)
{
	uint8_t value = 0;
	Set_Pin_Input(DS18B20);
	
	for(int i=0; i<8;i++) {
		 /*A read time slot is initiated by the master device pulling the 1-Wire bus low for a minimum of 1탎 and then releasing the bus */
		Set_Pin_Output(DS18B20);
		DS18B20_WritePin(DS18B20,0);
		Delay_us(1);
		
		/*The DS18B20 transmits a 1 by leaving the bus high and transmits a 0 by pulling the bus low*/
		Set_Pin_Input(DS18B20);
		if (DS18B20_ReadPin(DS18B20)==1){
			value|=(1<<i);
		}
		Delay_us(60);
	}
	return value;
}
/*----------------------------------------------------HIGH LEVEL LAYER------------------------------------------*/
float ConvertValueTemp(uint16_t Temp){
	float value = 0;
	int8_t check_negative = 1;
	if ((Temp&(1<<15))==1) {
		Temp = ~Temp+1U;
		check_negative=-1;
	}
	value = (float) Temp/16;
	value = value*check_negative;
	return value;
}

float DS18B20_Temp_Read(DS18B20_NAME* DS18B20){
	uint16_t Temp;
	float value;
	
	if (!DS18B20_Reset(DS18B20)) return 0;
	DS18b20_WriteData(DS18B20,SKIP_ROM);
	DS18b20_WriteData(DS18B20,CONVERT_T);
	
	if (!DS18B20_Reset(DS18B20)) return 0;
	DS18b20_WriteData(DS18B20,SKIP_ROM);
	DS18b20_WriteData(DS18B20,READ_SCRATCHPAD);
	
	Temp=DS18b20_ReadData(DS18B20);
	Temp|=DS18b20_ReadData(DS18B20)<<8;
	
	value = ConvertValueTemp(Temp);
	return value;
}
