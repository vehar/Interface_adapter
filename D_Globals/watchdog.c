#include <adapter.h>

//initialize watchdog
void WDT_Init(void)
{
#asm("cli")//disable interrupts
#asm("wdr")//reset watchdog
//set up WDT interrupt
WDTCR = (1<<WDCE)|(1<<WDE);
//Start watchdog timer with 2s prescaller
WDTCR = (1<<WDP2)|(1<<WDP1)|(1<<WDP0);
#asm("sei")//Enable global interrupts
}
/*
//Watchdog timeout ISR
ISR(WDT_vect)
{
    //Burst of fice 0.1Hz pulses
    for (uint8_t i=0;i<4;i++)
    {
        //LED ON
        PORTD|=(1<<PD2);
        //~0.1s delay
        _delay_ms(20);
        //LED OFF
        PORTD&=~(1<<PD2);
        _delay_ms(80);
    }
 */