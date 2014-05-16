#ifndef HAL_H
#define HAL_H



//Clock Config
  //in global_defines.h
#ifdef __CODEVISIONAVR__
    #define F_CPU  _MCU_CLOCK_FREQUENCY_
 #else
    #define F_CPU  16000000L
#endif


//System Timer Config
#define Prescaler	  		256                          //Upd-5
#define	TimerDivider  		(F_CPU/Prescaler/1000)		// 1 mS   //Upd-5


//USART Config
#define baudrate 38400L
#define bauddivider (F_CPU/(16*baudrate)-1)
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)


//PORT Defines
#define LED1 		6
#define LED2		7
#define	LED3		3

#define I_C			3
#define I_L			6
#define LED_PORT 	PORTD
#define LED_DDR		DDRD


//extern void InitAll(void);



#endif
