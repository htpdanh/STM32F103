#include "RTC.h"

static uint32_t count=0;
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

void RTC_main(void)
{	
	if (((RTC->CRL)& RTC_CRL_SECF) ==1U){
		count ++;
		RTC->CRL&= ~RTC_CRL_SECF;
	}
	if (count%2==0){
		GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	} else {
		GPIO_SetBits(GPIOC,GPIO_Pin_13);
	}
}

/* Bo vao ham main, dat bien count o main thanh static r run code*/
void RTC_IRQHandler(void)
{
	if (((RTC->CRL & RTC_CRL_SECF) ==1U) && ((RTC->CRH & RTC_CRH_SECIE)==1U)){
		count ++;
		RTC->CRL&= ~RTC_CRL_SECF;
		if (count%2==0){
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		} else {
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
		}
	}
}
