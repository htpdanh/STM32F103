#include "stm32f10x.h"
/*
- Counter register (TIMx_CNT): - Show gia tri dang dem 
- Prescaler register (TIMx_PSC): Cai dat tan so (f) cho clock
- Auto-Reload register (TIMx_ARR):- set up cho counter dem den 1 gtri nhat dinh nao do
- Set Bit CEN(Counter enable) in TIMx_CR1
- Set Bit UG(Update generation) tai TIMx_EGR
( Luu y doc bit UDIS(Update disable) de biet li do set bit UG
*/
void Timer2_Register(void);
void Delay_Timer2_Ms(uint32_t u32Delay);
void Timer2_Register_Interrupt(void);
void TIM2_IRQHandler(void);
void PWM_Timer2_One_Channel(void);
void PWM_Timer2_Multi_Channels(uint32_t SpeedCH1,uint32_t SpeedCH2,uint32_t SpeedCH3,uint32_t SpeedCH4);
void PWM_Standard(void);
