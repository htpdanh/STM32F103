#include "gpio.h"
#include "usart.h"
#include "stm32f1xx_hal.h"
#include "stdio.h"

void Parse_Uart_System_Init(void);

void Parse_Uart_Print_String(uint8_t *string);

uint32_t Parse_Uart_Find_Max_Data_Line();

/**
 *@brief Get status internal RX buffer
 *
 *@param[in] void
 *
 *@return bool - true: RX buffer have one or more uart line data
 *@return bool - false: RX buffer empty
 * */
_Bool Parse_Uart_Buffer_Ready(void);
