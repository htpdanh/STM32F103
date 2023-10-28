#include "stm32f10x.h"
#include "Timer.h"
/* USART2:
		PA2 - TX
		PA3 - RX */
void USART_Pin_Config(void);
void USART_Init_Config(uint32_t BaudRate);
void USART_Write(USART_TypeDef *USARTx, char*Str);
char* USART_Receive(USART_TypeDef *USARTx);
void USART_ReceiveIT_Setup(void);
void USART2_IRQHandler(void);

