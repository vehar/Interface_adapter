//***************************************************************************
//
//  Author(s)...: Vlad
//
//  Target(s)...: Mega
//
//  Compiler....:
//
//  Description.: Драйвер I2C
//
//  Data........:
//
//***************************************************************************
#include "I2C.h"


/*
TODO:
Hard/Software implementations

*/
////////////////HARDWARE_TWI/I2C///////////////////////////
///////////////////////////////////////////////////////////

void hard_twi_init(void){// TWI initialization
// Bit Rate: 400,000 kHz
TWBR=0x02;
// Two Wire Bus Slave Address: 0x0
// General Call Recognition: Off
TWAR=0x00;
// Generate Acknowledge Pulse: Off
// TWI Interrupt: On
TWCR = (1<<TWIE)|(1<<TWEN) ;
TWSR=0x00;
}


/*
void hard_twi_init(void);
void hard_twi_start(void);
void hard_twi_stop(void);
unsigned char hard_twi_read(unsigned char ack);
unsigned char hard_twi_write(unsigned char data);
*/

void hard_twi_start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send start condition
	while (!(TWCR & (1 << TWINT))){};
    //возможно сдесь нужно очистить  TWSTA
}

void hard_twi_write_byte(char byte) {
	TWDR = byte;
	TWCR = (1 << TWINT) | (1 << TWEN); // start address transmission
	while (!(TWCR & (1 << TWINT))){};
}

char hard_twi_read_byte() {
	TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN); // start data reception, transmit ACK
	while (!(TWCR & (1 << TWINT))){};
	return TWDR;
}

char hard_twi_read_last_byte() {
	TWCR = (1 << TWINT) | (1 << TWEN); // start data reception
	while (!(TWCR & (1 << TWINT))){};
	return TWDR;
}

void hard_twi_stop() {
	  TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // send stop condition
}

uint8_t hard_twi_read_ACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0){};
    return TWDR;
}
//read byte with NACK
uint8_t hard_twi_read_NACK(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0){};
    return TWDR;
}

uint8_t hard_twi_get_status(void)
{
    uint8_t status;
    status = TWSR & 0xF8;     //mask status
    return status;
}

/*
// Two Wire bus interrupt service routine
interrupt [TWI] void twi_isr(void)
{

TWCR = (1<<TWINT) ;// At the end - clear interrupt flag
}
*/