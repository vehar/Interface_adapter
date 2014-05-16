
#ifndef MY_TWI_H
#define MY_TWI_H


#include <mega16.h>
#include <delay.h>

#define SDA   1
#define SCL   0
#define I2C_PORT PORTC
#define I2C_DDR  DDRC
#define _BV(x) (1<<x)

char i2cTX_buf[128];
char i2cRX_buf[128];


void twi_init(void){
   I2C_PORT&=~_BV(SDA);
   I2C_PORT&=~-_BV(SCL);
   I2C_DDR &=~_BV(SDA);
   I2C_DDR &=~_BV(SCL);
}    

// Программный TWI   
#ifdef SOFT_TWI
unsigned char ack=0;
unsigned char data[5];
unsigned int Last_writen_addr=0;


void start_cond (void)
{
  PORTB=_BV(SDA)|_BV(SCL);
  #asm("nop");
  PORTB&=~_BV(SDA);
  #asm("nop");
  PORTB&=~_BV(SCL);
}

void stop_cond (void)
{
  PORTB=_BV(SCL);
  #asm("nop");
  PORTB&=~_BV(SDA);
  #asm("nop");
  PORTB|=_BV(SDA);
}

void send_byte (unsigned char data)
{ unsigned char i;
  for (i=0;i<8;i++)
   {
    if ((data&0x80)==0x00) {PORTB&=~_BV(SDA);}    //set SDA
    else PORTB|=_BV(SDA);
    #asm("nop");
    PORTB|=_BV(SCL);                //SCL impulse
    #asm("nop");
    PORTB&=~_BV(SCL);
    data=data<<1;
   }
   DDRB&=~_BV(SDA);
   PORTB|=_BV(SDA);

    #asm("nop");
    PORTB|=_BV(SCL);
    #asm("nop");
     if ((PINB&_BV(SDA))==_BV(SDA)) ack=1;    //Reading ACK
    PORTB&=~_BV(SCL);

   PORTB&=~_BV(SDA);
   DDRB|=_BV(SDA);
}

unsigned char get_byte (void)
{ unsigned char i, res=0;

  DDRB&=~_BV(SDA);
  PORTB|=_BV(SDA);
  for (i=0;i<8;i++)
   {
    res=res<<1;
    PORTB|=_BV(SCL);          //SCL impulse
    #asm("nop");
    if ((PINB&_BV(SDA))==_BV(SDA)){res=res|0x01;}  //Reading SDA
    PORTB&=~_BV(SCL);
    #asm("nop");
   }
  PORTB&=~_BV(SDA);
  DDRB|=_BV(SDA);
  #asm("nop");
  PORTB|=_BV(SCL);
  #asm("nop");
  PORTB&=~_BV(SCL);
 return res;
}

//example
/*
void axel_write_ (char adress_, char DATA_)
{
  start_cond();
  send_byte (adress_);
  send_byte (DATA_);
  stop_cond();
}
*/
    
#else
// Аппаратный TWI

#define IIC_START                   TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);   //Put Start Condition
#define PTD                         while(!(TWCR & (1<<TWINT)));            //Poll Till Done
#define TRANSMIT                    TWCR = (1<<TWINT)|(1<<TWEN);            //Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
#define CHK_DATA_TRANS_ACK_RES      if((TWSR & 0xF8) != 0x28) return 1;     //error  //Проверка статуса Отправки байта,получения мастером  ACK
#define TILL_SLA_W_TRANS_ACK_RES    while((TWSR & 0xF8) != 0x18);           // пока sla_address не будет передан и SLAVE не откликнется ACK-ом
#define CHK_START                   if((TWSR & 0xF8) != 0x08) return 1;     //Проверка статуса СТАРТ //error
#define CHK_REP_START               if((TWSR & 0xF8) != 0x10) return 1;     //Check status "A repeated START condition has been transmitted"
#define CHK_SLA_R_TRANS_ACK_RES     if((TWSR & 0xF8) != 0x40) return 1;     //Check status "SLA+R has been transmitted; ACK has been received"
#define ACK_TRANSMIT_DATA_RECEIVE   TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);    //Put ACK on TWI Bus и память отправляет следующий байт
#define DATA_RECEIVE                TWCR=(1<<TWINT)|(1<<TWEN);              // Просто разрешаем приём, очишая бит TWINT
#define IIC_STOP                    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWSTO);   //Put Stop Condition on bus
#define TILL_STOP_FINISH            while(TWCR & (1<<TWSTO));               //Wait for STOP to finish

#endif  

#endif //MY_TWI_H 