#include "SPI_practice.h"

void SPI_SendData(SPI_TypeDef* SPIx, uint8_t *pTxBuffer, uint32_t len)
{
	while (len>0)
	{
		//1.Wait until TXE is set (means empty)
		while (SPI_I2S_GetFlagStatus(SPIx,SPI_FLAG_TXE) == RESET);
		//check data frame 16 bits or 8 bits
		if (SPIx->CR1 & SPI_CR1_DFF)
		{
			//with data frame 16 bits
			SPIx->DR = *((uint16_t*)pTxBuffer);
			len--;
			len--;
			(uint16_t*)pTxBuffer++;
		}
		else {
			//with data frame 8 bits
			SPIx->DR = *pTxBuffer;
			len--;
			pTxBuffer++;
		}
	}
}
