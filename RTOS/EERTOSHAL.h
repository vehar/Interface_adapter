#ifndef EERTOSHAL_H
#define EERTOSHAL_H
#include <mega128.h>
#include <stdint.h>
#include <stdlib.h>    // itoa
#include <mem.h>

#define STATUS_REG 			    SREG
#define Interrupt_Flag		    SREG_I
#define _disable_interrupts()	#asm("cli")
#define _enable_interrupts()	#asm("sei")
#define wdt_reset()             #asm("wdr")
       
#ifdef __CODEVISIONAVR__
    #define RTOS_ISR  		TIM2_COMP
    #define DEAD_TIME_ISR   TIM0_COMP        
    #define F_CPU  _MCU_CLOCK_FREQUENCY_
 #else
    #define RTOS_ISR  		TIMER2_COMP_vect   
    #define DEAD_TIME_ISR   TIMER0_COMP_vect   
    #define F_CPU  16000000L
#endif

//RTOS Config 
//System Timer Config
#define Prescaler	  		    256                 
#define	RtosTimerDivider  		(F_CPU/Prescaler/1000)// 1 mS   
//#define	RtosTimerDivider  	(F_CPU/Prescaler/10000)*/// 0.1mS! 
//Прерывание 10 000 раз в сек., но обработчик таймеров стартует каждое 10-е прерывание
//получается 1мс

#define	DeadTimerDivider  		(F_CPU/Prescaler/1000)
#define HI(x) ((x)>>8)
#define LO(x) ((x)& 0xFF)

extern void RTOS_timer_init (void);
extern void RunRTOS (void);      //запуск системмного таймера
extern void StopRTOS (void);     //замедление системмного таймера
extern void FullStopRTOS (void); //полная остановка системмного таймера

extern void DeadTimerInit (void);
extern void DeadTimerRun (void);
extern void DeadTimerStop (void);

 extern void Timer_3_start(void);
 extern void Timer_3_stop(void);   
 extern uint16_t Timer_3_get_val (void);    
 
//PORT Defines
#define LED1 		6
#define LED2		7
#define	LED3		5

#define I_C			3
#define I_L			6
#define LED_PORT 	PORTD
#define LED_DDR		DDRD

#endif
