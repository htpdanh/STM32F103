#include "stm32f10x.h"
#include "stdio.h"
void RTC_Register(void);
void GPIO_Config(void);
void RTC_Config(void);
void RTC_IRQHandler(void);

typedef struct {
	uint8_t u8Hours;
	uint8_t u8Minutes;
	uint8_t u8Seconds;
	uint8_t u8Week;
//	uint8_t u8Hours_Alarm;
//	uint8_t u8Minutes_Alarm;
//	uint8_t u8Seconds_Alarm;
//	uint8_t u8Week_Alarm;
}Clock_TypeDef;

//static uint32_t count=0;
static Clock_TypeDef Clock;
int main(void)
{
	//Clock Init
	Clock.u8Hours=0; Clock.u8Minutes=0;Clock.u8Seconds=0;Clock.u8Week=2U;
	
	GPIO_Config();
	RTC_Config();
	while (1)
	{
	}
}

void GPIO_Config(void)
{ 
   GPIO_InitTypeDef  GPIO_InitStructure; 
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
   GPIO_Init(GPIOC, &GPIO_InitStructure); 
}

void RTC_Config(void)
{
		/*enable the power and backup interface clocks by setting the PWREN and BKPEN bits in the RCC_APB1ENR register*/
	RCC->APB1ENR|=RCC_APB1ENR_PWREN;
	RCC->APB1ENR|=RCC_APB1ENR_BKPEN;
	/*set the DBP bit the Power Control register (PWR_CR) to enable access to the Backup registers and RTC.*/
	PWR->CR|=PWR_CR_DBP;
	
	/* Enable LSE CLOCK */
	RCC->BDCR|=RCC_BDCR_LSEON;
	/* Wait until the LSE clock ready */
	while (((RCC->BDCR>>1U) & 0x01U)!=1U);
	
	/*RTC clock source selection*/
	RCC->BDCR|=RCC_BDCR_RTCSEL_LSE;
	
	/*RTC clock enable*/
	RCC->BDCR|=RCC_BDCR_RTCEN;
	
	/*Configuration procedure
	1. Poll RTOFF, wait until its value goes to ‘1’
	2. Set the CNF bit to enter configuration mode
	3. Write to one or more RTC registers
	4. Clear the CNF bit to exit configuration mode
	5. Poll RTOFF, wait until its value goes to ‘1’ to check the end of the write operation*/
	while (((RTC->CRL>>5) & 0x01U) !=1U); //1
	RTC->CRL|=RTC_CRL_CNF; //2
	/* 3. RTC prescaler (bo chia de nhan dc xung cua clock */
	RTC->PRLH=0U;
	
	/* Neu muon tao ra ngat trong RTC thi ENABLE flag SECIE va set flag SECF = 0 theo nhu RTC functional description */
	RTC->CRH|=RTC_CRH_SECIE;
	RTC->CRL&= ~RTC_CRL_SECF;

	RTC->PRLL=0x7FFF; //If the input clock frequency (fRTCCLK) is 32.768 kHz, write 7FFFh (32767) in this register to get a signal period of 1 second.
	//RTC->CRL &= ~0x10U; //4
	while (((RTC->CRL>>5) & 0x01U) !=1U);
	NVIC->ISER[0]=0x08;
}

void RTC_IRQHandler(void)
{
	if (((RTC->CRL & RTC_CRL_SECF) ==1U) && ((RTC->CRH & RTC_CRH_SECIE)==1U)){
		//count ++;
		RTC->CRL&= ~RTC_CRL_SECF;
		Clock.u8Seconds++;
		if (Clock.u8Seconds>=60U){
			Clock.u8Seconds=0;
			Clock.u8Minutes++;
			if (Clock.u8Minutes>=60U){
				Clock.u8Minutes=0;
				Clock.u8Hours++;
				if (Clock.u8Hours>=24U){
					Clock.u8Hours=0;
					Clock.u8Week++;
					if (Clock.u8Week==9U){
						Clock.u8Week=2U;
					}
				}
			}
		}
	}
}
	
