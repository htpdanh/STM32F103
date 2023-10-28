#ifndef __IR_H
#define __IR_H

#include "stm32f1xx_hal.h"
#define START_TIME_MAX 140
#define START_TIME_MIN 120
typedef struct
{	
	uint32_t HexCode;
	TIM_HandleTypeDef* Timer;
	uint32_t Channel;
}IR_Name;

void IR_Init(IR_Name* IR, TIM_HandleTypeDef* Timer, uint32_t Channel);
uint32_t IR_ReadCode(IR_Name* IR);
#endif
