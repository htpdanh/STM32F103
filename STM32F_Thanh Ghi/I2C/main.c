#include "main.h"

#define SLAVE_ADDR 0x68
void I2C_MasterSendData(I2C_TypeDef* I2Cx,uint8_t *pTxbuffer, uint8_t Len, uint8_t SlaveAdd);

uint8_t somedata[]="We are testing I2C master Tx\n";

int main(void)
{
	Timer2_StandardLibrary();
	I2C_GPIO_Config();
	I2C_Init_Config();
	while (1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
		Delay_Timer2_StandardLibrary_Ms(1000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		Delay_Timer2_StandardLibrary_Ms(1000);
		
		I2C_MasterSendData(I2C2,somedata,strlen((char*)somedata),0x68);
		Delay_Timer2_StandardLibrary_Ms(1000);
		/*I2C_GenerateSTART(I2C2,ENABLE);
		while(! I2C_GetFlagStatus(I2C2, I2C_FLAG_SB) );
		I2C_Send7bitAddress(I2C2,0,I2C_Direction_Transmitter);
		while (! I2C_GetFlagStatus(I2C2,I2C_FLAG_ADDR) );
		I2C_SendData(I2C2,0x80);
		while (! I2C_GetFlagStatus(I2C2,I2C_FLAG_TXE) );
		while (! I2C_GetFlagStatus(I2C2,I2C_FLAG_BTF) );
		I2C_GenerateSTOP(I2C2,ENABLE);*/
		
	}
}

void I2C_MasterSendData(I2C_TypeDef* I2Cx,uint8_t *pTxbuffer, uint8_t Len, uint8_t SlaveAdd)
{
	//1.Generate the Start condition
	I2C_GenerateSTART(I2Cx,ENABLE);
	
	//2. confirm that start generation is completed by checking the SB flag in the SR1
	//while(! I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) );
	
	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_Send7bitAddress(I2Cx, SlaveAdd, I2C_Direction_Transmitter);
	
	//4. Confirm that address phase is completed by checking the ADDR flag in the SR1
	//while (! I2C_GetFlagStatus(I2Cx,I2C_FLAG_ADDR) );
	
	//5. clear the ADDR flag according to its software sequence
	//6. send the dât until len becomes 0
	while (Len>0)
	{
		//while (! I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE) );
		I2Cx->DR=*pTxbuffer;
		pTxbuffer++;
		Len--;
	}
	//7. When Len becomes zero wait for TXE=1 and BTF=1 before generating the STOP condition
	//	Note: TXE=1 , BTF=1 , means that both SR and DR are empty and next transmission should begin
	//	When BTF=1 SCL will be stretched (pulled to LOW)
	
	//while (! I2C_GetFlagStatus(I2Cx,I2C_FLAG_TXE) );
	
	//while (! I2C_GetFlagStatus(I2Cx,I2C_FLAG_BTF) );
	
	//8. Generater STOP condition and master need not tot wait for the completion of stop condition
	//	Note: generating STOP, automatically clears the BTF.
	I2C_GenerateSTOP(I2Cx,ENABLE);
}
