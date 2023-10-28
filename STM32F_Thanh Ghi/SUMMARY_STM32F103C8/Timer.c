#include "Timer.h"

volatile uint32_t u32TimerInterrupt=0;

void Timer2_Register(void)
{
	/*RCC->APB1ENR|=RCC_APB1Periph_TIM2;
	TIM2->PSC=72-1;	//	72/(72-1+1)=1MHz
	TIM2->ARR=0xFFFF;
	TIM2->CR1=0x01;//counter enable
	TIM2->EGR=0x01;//Generate an update event to reload the Prescaler and the Repetition counter values immediately
	*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_TimeBaseInitTypeDef TIMx;
	
	TIMx.TIM_Prescaler=72-1;
	TIMx.TIM_CounterMode=TIM_CounterMode_Up;
	TIMx.TIM_Period=0xFFFF;
	
	TIM_TimeBaseInit(TIM2,&TIMx);
	
	TIM_Cmd(TIM2,ENABLE);
}
void Delay_Timer2_Ms(uint32_t u32Delay)
{
	while (u32Delay>0){
		/*TIM2->CNT=0U;
		while ((TIM2->CNT)<1000);
		u32Delay--;*/
		TIM_SetCounter(TIM2,0);
		while (TIM_GetCounter(TIM2)<1000);
		u32Delay--;
	}
}

void Timer2_Register_Interrupt(void)
{
	/* Enable clock for timer 2*/
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;
	/* PSC[15:0]: Prescaler value*/
	TIM2->PSC=7200U-1U;	//	72/(72-1+1)=1MHz
	/* IMx auto-reload register (TIMx_ARR) */
	TIM2->ARR=20000-1;
	/* Bit 0 CEN: Counter enable and 0: Counter used as upcounter*/
	TIM2->CR1=0x01;//counter enable
	/*Generate an update event to reload the Prescaler and the Repetition counter values immediately*/
	TIM2->EGR=0x01;
	/* Bit 0 UIE: Update interrupt enable*/
	TIM2->DIER|=TIM_DIER_UIE;
	/* Clear interrupt flag */
	TIM2->SR&= ~(0x01);
	/* counter =0 */
	TIM2->CNT = 0U;
	/* 28 settable TIM2 interrupt*/
	NVIC->ISER[0]=0x10000000;
}

void TIM2_IRQHandler(void)
{
	if (((TIM2->SR & 0x01U) != 0U) && (((TIM2->DIER)&0x01U) == 0x01U))
	{
		TIM2->SR &= ~(0x01U);
		u32TimerInterrupt++;
		if (u32TimerInterrupt%2U == 0)
		{
			GPIO_ResetBits(GPIOC,GPIO_Pin_13);
		}
		else {
			GPIO_SetBits(GPIOC,GPIO_Pin_13);
		}
	}
}

/* Khi thay doi ARR hoac PSC ta se thiet lap nen 1 chu ki T moi, vi vay no se quyet dinh tan so dau vao cua he thong
	 Trong khi do CCR1 se quyet dinh ti le phan tram giua xung cao va xung thap*/

void PWM_Timer2_One_Channel(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 720-1;
	TIM2->ARR = 10;
	TIM2->CR1 = TIM_CR1_CEN;
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CNT = 0U;
	/*Chon loai PWM (PW1 or PWM2)*/
	TIM2->CCMR1|=0x60;
	/*chon Ouput cho PWM
	CC1E - enable for output or input of PWM
	CC1P - compare output polarity*/
	TIM2->CCER|=0x01;
	/*set ti le % giua xung thap va xung cao (chinh la cuong do dong dien manh hay yeu...)*/
	TIM2->CCR1|=(10*(uint32_t)(TIM2->ARR))/100;
}

/* Config cho 1 hoac 4 channel thi phai cofig o thanh CCMR1 (channel 1 va channel 2) CCMR2 (channel 3 va channel 4) 
	 Ti le phan tram giua xung thap va xung cao cua cac channel dc config tai cac thanh ghi CCR1 (channel 1) CCR2 (ch2) CCR3,CCR4
	 Tai thanh ghi CCER cung se chia ra lam 4 phan tuong ung voi 4 channel */
void PWM_Timer2_Multi_Channels(uint32_t SpeedCH1,uint32_t SpeedCH2,uint32_t SpeedCH3,uint32_t SpeedCH4)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->PSC = 7200-1;
	TIM2->ARR = 10000;
	TIM2->CR1 = TIM_CR1_CEN;
	TIM2->EGR |= TIM_EGR_UG;
	TIM2->CNT = 0U;
	/*Chon loai PWM1 for Channel 1 & 2*/
	TIM2->CCMR1|=0x6060;
	/*Chon loai PWM1 for Channel 3 & 4*/
	TIM2->CCMR2|=0x6060;
	/*chon Ouput cho PWM*/
	TIM2->CCER|=0x1111;
	
	/*PWM channel 1*/
	TIM2->CCR1|=(SpeedCH1*(uint32_t)(TIM2->ARR))/100;
	
	/*PWM channel 2*/
	TIM2->CCR2|=(SpeedCH2*(uint32_t)(TIM2->ARR))/100;
	
	/*PWM channel 3*/
	TIM2->CCR3|=(SpeedCH3*(uint32_t)(TIM2->ARR))/100;
	
	/*PWM channel 4*/
	TIM2->CCR4|=(SpeedCH4*(uint32_t)(TIM2->ARR))/100;
}

void PWM_Standard(void)
{
	TIM_TimeBaseInitTypeDef TimerInit;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TimerInit.TIM_CounterMode=TIM_CounterMode_Up;
	TimerInit.TIM_Period=10000-1;
	TimerInit.TIM_Prescaler=7200-1;
	
	TIM_TimeBaseInit(TIM2,&TimerInit);
	
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_OCInitTypeDef pwmInit;
	/* Ch2 with duty 10% */
	/* PWMode 1 ung voi 10% se la muc cao, PWmod 2 thi nguoc lai */
	pwmInit.TIM_OCMode=TIM_OCMode_PWM1;
	pwmInit.TIM_OCNPolarity=TIM_OCPolarity_High;//quyet dinh TIM_pulse la xung cao
	pwmInit.TIM_Pulse=(10*10000)/100; //phan tram giua xung cao hay xung thap
	pwmInit.TIM_OutputState=TIM_OutputState_Enable;//chon pwm output 
	TIM_OC2Init(TIM2,&pwmInit);
}
