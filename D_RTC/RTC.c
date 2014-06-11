/**** A V R  A P P L I C A T I O N  NOTE 1 3 4 **************************
 *
 * Title:           Real Time Clock
 * Version:         2.00
 * Last Updated:    24.09.2013
 * Target:          ATmega128 (All AVR Devices with secondary external oscillator)
 *
 * Support E-mail:  avr@atmel.com
 *
 * Description
 * This application note shows how to implement a Real Time Clock utilizing a secondary
 * external oscilator. Included a test program that performs this function, which keeps
 * track of time, date, month, and year with auto leap-year configuration. 8 LEDs are used
 * to display the RTC. The 1st LED flashes every second, the next six represents the
 * minute, and the 8th LED represents the hour.
 *
 ******************************************************************************************/

#ifdef _GCC_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#else

#endif

#include "RTC.h"

	time_t rtc; 

/*
int main(void)
{
    rtc_init();	//Initialize registers and configure RTC.

	while(1)
	{
		sleep_mode();										//Enter sleep mode. (Will wake up from timer overflow interrupt)
		TCCR0=(1<<CS00)|(1<<CS02);							//Write dummy value to control register
		while(ASSR&((1<<TCN0UB)|(1<<OCR0UB)|(1<<TCR0UB)));	//Wait until TC0 is updated
	}
}
*/
inline void RTC_init(void)
{
   	TIMSK &= ~((1<<TOIE0)|(1<<OCIE0));						//Make sure all TC0 interrupts are disabled
	ASSR |= (1<<AS0);										//set Timer/counter0 to be asynchronous from the CPU clock
															//with a second external clock (32,768kHz)driving it.
	TCNT0 =0;												//Reset timer
	TCCR0 =(1<<CS00)|(1<<CS02);								//Prescale the timer to be clock source/128 to make it
															//exactly 1 second for every overflow to occur
	while (ASSR & ((1<<TCN0UB)|(1<<OCR0UB)|(1<<TCR0UB))){ }	//Wait until TC0 is updated
	TIMSK |= (1<<TOIE0);									//Set 8-bit Timer/Counter0 Overflow Interrupt Enable
#asm("sei")													//Set the Global Interrupt Enable Bit
}

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
	if (++rtc.second==60)        //keep track of time, date, month, and year
	{
		rtc.second=0;
		if (++rtc.minute==60)
		{
			rtc.minute=0;
			if (++rtc.hour==24)
			{
				rtc.hour=0;
				if (++rtc.date==32)
				{
					rtc.month++;
					rtc.date=1;
				}
				else if (rtc.date==31)
				{
					if ((rtc.month==4) || (rtc.month==6) || (rtc.month==9) || (rtc.month==11))
					{
						rtc.month++;
						rtc.date=1;
					}
				}
				else if (rtc.date==30)
				{
					if(rtc.month==2)
					{
						rtc.month++;
						rtc.date=1;
					}
				}
				else if (rtc.date==29)
				{
					if((rtc.month==2) && (not_leap()))
					{
						rtc.month++;
						rtc.date=1;
					}
				}
				if (rtc.month==13)
				{
					rtc.month=1;
					rtc.year++;    // HAPPY NEW YEAR !  :)
				}
			}
		}
	}
	//PORTB=~(((t.second&0x01)|t.minute<<1)|t.hour<<7);
}

static char not_leap(void)      //check for leap year
{
	if (!(rtc.year%100))
	{
		return (char)(rtc.year%400);
	}
	else
	{
		return (char)(rtc.year%4);
	}
}

//**************************************************************
// read RTC function
void getRTC(time_t* stm)
 {
  __disable_interrupts();     // evite erronated read because RTC is called from interrupt
  memcpy(stm,&rtc,sizeof(time_t));
  __restore_interrupts();
 }
//**************************************************************
// set RTC function
/*
void setRTC(u16 year, u08 mon, u08 day, u08 hour, u08 min, u08 sec)
 {
  __disable_interrupts();
  rtc.year=year;
  rtc.month=mon;
  rtc.date=day;
  rtc.hour=hour;
  rtc.minute=min;
  rtc.second=sec;
  __restore_interrupts();
 }*/
 
 void setRTC(time_t* rtc_p, u16 year, u08 mon, u08 day, u08 hour, u08 min, u08 sec)
 {
  __disable_interrupts();
  rtc_p->year=year;
  rtc_p->month=mon;
  rtc_p->date=day;
  rtc_p->hour=hour;
  rtc_p->minute=min;
  rtc_p->second=sec;
  __restore_interrupts();
 }