#include <adapter.h>

inline void GPIO_init(void){
PORTA=0x00; DDRA=0x00;
PORTB=0x00; DDRB=0x07;
PORTC=0x00; DDRC=0x00;
PORTD=0x00; DDRD=0x00;
PORTE=0x00; DDRE=0x00;
PORTF=0x00; DDRF=0x00;
PORTG=0x00; DDRG=0x00;
}


inline void TIM_0_init(void){// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
// Mode: Normal top=0xFF
// OC0 output: Disconnected
ASSR=0x00;
TCCR0=0x00;
TCNT0=0x00;
OCR0=0x00;
}

inline void TIM_1_init(void){// Timer/Counter 1 initialization
TCCR1A=0x00;    TCCR1B=0x00;
TCNT1H=0x00;    TCNT1L=0x00;
ICR1H=0x00;     ICR1L=0x00;
OCR1AH=0x00;    OCR1AL=0x00;
OCR1BH=0x00;    OCR1BL=0x00;
OCR1CH=0x00;    OCR1CL=0x00;
}

inline void TIM_2_init(void){// Timer/Counter 2 initialization
TCCR2=0x00;
TCNT2=0x00;
OCR2=0x00;
}

inline void TIM_3_init(void){ // Timer/Counter 3 initialization
TCCR3A=0x00;    TCCR3B=0x00;
TCNT3H=0x00;    TCNT3L=0x00;
ICR3H=0x00;     ICR3L=0x00;
OCR3AH=0x00;    OCR3AL=0x00;
OCR3BH=0x00;    OCR3BL=0x00;
OCR3CH=0x00;    OCR3CL=0x00;
}

inline void INT_init(void){// External Interrupt(s) initialization
// INTx: Off
EICRA=0x00;
EICRB=0x00;
EIMSK=0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
TIMSK=0x00;
ETIMSK=0x00;
}


inline void USART_0_init(void){// USART0 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART0 Receiver: On
// USART0 Transmitter: On
// USART0 Mode: Asynchronous
// USART0 Baud Rate: 9600
UCSR0A=0x00;
UCSR0B=0xD8;
UCSR0C=0x06;
UBRR0H=0x00;
UBRR0L=0x33;
}

inline void USART_1_init(void){// USART1 initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART1 Receiver: On
// USART1 Transmitter: On
// USART1 Mode: Asynchronous
// USART1 Baud Rate: 115200 (Double Speed Mode)
UCSR1A=0x02;
UCSR1B=0xD8;
UCSR1C=0x06;
UBRR1H=0x00;
UBRR1L=0x08;
}

/*
void SPI_init(void){// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2000,000 kHz
// SPI Clock Phase: Cycle Start
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=0xD0;
SPSR=0x00;
// Clear the SPI interrupt flag
#asm
    in   r30,spsr
    in   r30,spdr
#endasm
}*/

inline void TWI_init(void){// TWI initialization
// Bit Rate: 400,000 kHz
TWBR=0x02;
// Two Wire Bus Slave Address: 0x0
// General Call Recognition: Off
TWAR=0x00;
// Generate Acknowledge Pulse: Off
// TWI Interrupt: On
TWCR=0x05;
TWSR=0x00;
}

void settings_EE_cpy_R(void){ // settings transfer from eeprom to ram
uint8_t i = 0;
  for(i=0;i<COUNT_OF_UARTS;i++)
    {
        RAM_settings.MODE_of_Uart[i] = EE_settings.MODE_of_Uart[i];
        RAM_settings.baud_of_Uart[i] = EE_settings.baud_of_Uart[i];
    }
  for(i=0;i<COUNT_OF_SPI;i++)
    {
        RAM_settings.MODE_of_Spi[i] = EE_settings.MODE_of_Spi[i];
        RAM_settings.prescaller_of_Spi[i] =  EE_settings.prescaller_of_Spi[i];
    }
}

inline void First_EE_init(void){ // settings transfer from eeprom to ram
uint8_t i = 0;
  for(i=0;i<COUNT_OF_UARTS;i++)
    {
EE_settings.MODE_of_Uart[i] = USART_NORMAL;
    }
    EE_settings.baud_of_Uart[0] = 576; //57600baud
    EE_settings.baud_of_Uart[1] = 288; //28800baud
  for(i=0;i<COUNT_OF_SPI;i++)
    {
EE_settings.MODE_of_Spi[i] = 0;
EE_settings.prescaller_of_Spi[i] = 16;
    }
}


void HARDWARE_init(void)
{
 GPIO_init();
 ADC_init(); //Upd-6
 ADC_calibrate(); //Upd-7
 TIM_0_init();
 TIM_1_init();
 TIM_2_init();
 TIM_3_init();
 INT_init();
 USART_0_init();
 USART_1_init();
 Hard_SPI_Master_Init_default();
//TWI_init();
 RTC_init(); //Timer 0 used

//i2c_init(); // I2C Bus initialization
//w1_init(); // 1 Wire Bus initialization
// 1 Wire Data port: PORTA
// 1 Wire Data bit: 2
// Note: 1 Wire port settings must be specified in the
// Project|Configure|C Compiler|Libraries|1 Wire IDE menu.
}

void SOFTWARE_init (void)
{
//#ifdef EEPROM_REINIT  //Upd-5
if(null_ee != 0x42)	// если первое включение - инициализируем переменные ЕЕПРОМ
   {
    First_EE_init();  //начальная инициализация еепром (выполняется 1 раз) 
    null_ee = 0x42;
   }
//#endif
settings_EE_cpy_R(); //загрузка настроек из еепром

// check_after_pow_on();
// flags_init();

//WDT_Init();//Watchdog

USART_Init(USART_0, RAM_settings.MODE_of_Uart[USART_0], RAM_settings.baud_of_Uart[USART_0]);
USART_Init(USART_1, RAM_settings.MODE_of_Uart[USART_1], RAM_settings.baud_of_Uart[USART_1]);

#ifdef DEBUG
USART_Init(USART_0, 1, 576);
#endif

//Soft_SPI_Master_Init();
//Hard_SPI_Master_Init_default();
#warning грузить из еепром!
SPI_init(SPI_0, SPI_MASTER, 1, 0, 16);
LcdInit();//Upd-3 //soft spi on portA

//cust_I2C_init(I2C_0);

//======
Init_i2c();						// Запускаем и конфигурируем i2c
Init_Slave_i2c(&SlaveControl);	// Настраиваем событие выхода при сработке как Slave
//======

// Clear the SPI interrupt flag   //Upd-4
#asm
    in   r30,spsr
    in   r30,spdr
#endasm


PARSER_Init();



#ifdef DEBUG
LogIndex=0;					// Лог с начала
WorkLog[LogIndex]=1;		// Записываем метку старта
LogIndex++;
#endif
}




























