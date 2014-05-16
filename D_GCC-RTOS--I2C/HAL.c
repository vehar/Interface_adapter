#include <HAL.h>

inline void InitAll(void)
{

//InitUSART
UBRRL = LO(bauddivider);
UBRRH = HI(bauddivider);
UCSRA = 0;
UCSRB = 1<<RXEN|1<<TXEN|1<<RXCIE|0<<TXCIE|0<<UDRIE;
UCSRC = 1<<URSEL|1<<UCSZ0|1<<UCSZ1;

//InitPort
PORTB = 0x00;
DDRB = 1;


//Init ADC


//Init PWM


//Init Interrupts


//Init Timers

}
