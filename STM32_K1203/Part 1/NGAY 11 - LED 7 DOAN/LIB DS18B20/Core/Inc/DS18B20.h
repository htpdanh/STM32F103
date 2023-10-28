#ifndef DS18B20_H
#define DS18B20_H

#include "stm32f1xx_hal.h"
#include "math.h"

typedef struct{
	GPIO_TypeDef* Port;  
	uint16_t Pin;
}DS18B20_NAME;

#define SKIP_ROM 0xCC
#define CONVERT_T 0x44
#define READ_SCRATCHPAD 0xBE

/*-------ADDITIONAL COMMANDS---------*/
#define SEARCH_ROM 0xF0
#define READ_ROM 0x33
#define	MATCH_ROM 0x55
#define ALARM_SEARCH 0xEC
#define WRITE_SCRATCHPAD 0x4E
#define COPY_SCRATCHPAD 0x48
#define RECALL_E2 0xB8
#define READ_POWER_SUPPLY 0xB4

void Delay_us(uint32_t us);
void DS18B20_Init(DS18B20_NAME* DS18B20,GPIO_TypeDef* DS18B20_PORT,uint16_t DS18B20_PIN);
void DS18B20_WritePin(DS18B20_NAME* DS18B20,uint8_t PinState);
uint8_t DS18B20_ReadPin(DS18B20_NAME* DS18B20);
void Set_Pin_Output(DS18B20_NAME* DS18B20);
void Set_Pin_Input(DS18B20_NAME* DS18B20);
uint8_t DS18B20_Reset(DS18B20_NAME* DS18B20); 
void DS18b20_WriteData(DS18B20_NAME* DS18B20,uint8_t Data);
uint8_t DS18b20_ReadData(DS18B20_NAME* DS18B20);
float ConvertValueTemp(uint16_t Temp);
float DS18B20_Temp_Read(DS18B20_NAME* DS18B20);

#endif
