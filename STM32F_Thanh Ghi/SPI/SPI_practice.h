#include "stm32f10x.h"
#include "stm32f10x.h"
#include "stdio.h"

#define SPI_FLAG_RXNE               ((uint16_t)0x0001)
#define SPI_FLAG_TXE                ((uint16_t)0x0002)
#define SPI_FLAG_BSY                ((uint16_t)0x0080)
void SPI_SendData(SPI_TypeDef* SPIx, uint8_t *pTxBuffer, uint32_t len);
