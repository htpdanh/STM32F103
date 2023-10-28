#include "SysTick.h"
#include "math.h"

double fTimeMeasurement =0.0;

/*
De thiet lap ham delay cho SysTick can phai quan tam cac tham so sau:
- STK_CTRL->CLKSOURCE de thiet lap tan so (f) cho chan
- STK_CTRL->ENABLE de enable bo dem (counter) cua SysTick
- STK_LOAD de set gia tri dem nguoc ve cua SysTick
- Dat lai gia tri tai STK_VAL = 0 de ngung dem khi gia tri ve 0
- Kiem tra co ngat (Count FLAG) tai STK_CTRL
*/

void Delay_Ms(uint32_t nCount)
{
	while (nCount>0){
		SysTick->CTRL|=SysTick_CTRL_CLKSOURCE;//use processor clock AHB (72MHz)
		SysTick->CTRL|=SysTick_CTRL_ENABLE;//enable counter
		SysTick->LOAD = 72U*1000-1U;//boi vi f=72MHz=72*10^6 va T=1/f => moi 1Ms=1/1000 Mhz => voi f=72MHz ta phai LOAD tu gtri 72000 de dc 1Ms
		SysTick->VAL = 0U;//K biet de lam gif 
		while (!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG));
		nCount--;
	}
}
	
/*STK_VAL:
Current counter value
The VAL register contains the current value of the SysTick counter.
Reads return the current value of the SysTick counter.
A write of any value clears the field to 0, and also clears the COUNTFLAG bit in the STK_CTRL register to 0.*/

/*
SysTick Interrupt
de kick hoat interrupt thi set bien TICKINT len 1.

Phan nay ta tao start_breakpoint and stop_breakpoint de tinh thoi gian chay cua 1 doan ctr mat bao nhieu thoi gian
*/
void Start_breakpoint(void)
{
	SysTick->LOAD=72U*1000-1U; //dem nguoc tu 1Ms ve 0
	SysTick->CTRL|=SysTick_CTRL_ENABLE;
	SysTick->CTRL|=SysTick_CTRL_CLKSOURCE;
	SysTick->CTRL|=SysTick_CTRL_TICKINT; //khi tong thoi gian dem nguoc >=1Ms thi xay ra ngat (interrupt)
	SysTick->VAL=0U;
}
void Stop_breakpoint(void)
{
	fTimeMeasurement = fTimeMeasurement+(71999U-(SysTick->VAL))/71999.0;
	SysTick->CTRL&=0;
}
void SysTick_Handler(void){
	fTimeMeasurement +=1;
}
