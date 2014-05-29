
#ifndef GLOBAL_DEFINES
#define GLOBAL_DEFINES

#ifdef __CODEVISIONAVR__
    #define F_CPU  _MCU_CLOCK_FREQUENCY_ //in CV
#else
    #define F_CPU  16000000UL
#endif

#define array_size(arr) (sizeof(arr)/sizeof(arr[0]))
#define TCNT1_1MS (65536-(F_CPU/(256L*1000))) //столько тиков будет делать T/C1 за 1 мс
  /* void SetupTIMER1 (void){ //example    
  #asm("cli");      //Upd-11
  TCCR1B = (1<<CS12);
  TCNT1 = TCNT1_1MS;
  TIMSK = (1<<TOIE1);}   // Enable timer 1 overflow interrupt.   
  #asm("sei");      //Upd-11
  */


//--------------START UART DEFINES---------------//


/*
#define FRAMING_ERROR       (1<<FE)
#define PARITY_ERROR        (1<<UPE)
#define DATA_OVERRUN        (1<<DOR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE         (1<<RXC)
*/
//#define RX_BUFFER_SIZE 128     // USART Receiver buffer
//--------------END UART DEFINES---------------//


//--------------START SPI DEFINES---------------//
#define COUNT_OF_SPI 2
//--------------END SPI DEFINES---------------//


//--------------START I2C DEFINES---------------//
//#define COUNT_OF_I2C 2  //UPD - in I2C.h
//--------------END I2C DEFINES---------------//



//--------------START ADC DEFINES---------------//
#define ADC_VREF_TYPE 0xC0
//--------------END ADC DEFINES---------------//

//--------------START MACROS---------------//

	//#define MIN(A,B)       ((A) <=  (B) ? (A) : (B))        bad
  //  inline int Min(int a, int b) //good
//--------------END MACROS---------------//


//----START OTHERS----//

extern volatile uint8_t saved_state = 0;
#warning заменить на push pop
#define __disable_interrupts() do{saved_state = SREG;#asm("cli");}while(0)
#define __restore_interrupts() (SREG |= saved_state)


#define DDRX   DDRB
#define PORTX PORTB

#define _set(X) (X=1)
#define _clr(X) (X=0)
#define _read(X)(X)

#define L_RISING_EDGE    GICR|=(1<<INT0);MCUCR|=((1<<ISC00)|(1<<ISC01))
#define L_FALLING_EDGE   GICR|=(1<<INT0);MCUCR&=~(1<<ISC00);MCUCR|=(1<<ISC01))
#define L_OFF            GICR&=~(1<<INT0);MCUCR&=~((1<<ISC00)|(1<<ISC01))
#define _set_EXT_INT(X)  (X)


//----END OTHERS----//

/*

  Для того чтобы текст программы легче читался, он также должен быть разбит на части (секции),
  каждая из которых имеет свое назначение:

Заголовок файла (с информацией о назначении файла, авторе текста, используемом компиляторе, дате создания и пр.);
Хронологию изменений
Секция включаемых файлов
Секция определения констант
Секция определения макросов
Секция определения типов
Секция определения переменных (глобальные, затем локальные)
Секция прототипов функций (глобальные, затем локальные)
Секция описания функций.
Для секций есть свои правила:

Каждая секция должна содержать только те описания, которые ей соответствуют
Секции должны быть едины, а не разбросаны по всему файлу.
Каждой секции должен предшествовать хорошо заметный блок комментария с названием секции.
*/
#endif