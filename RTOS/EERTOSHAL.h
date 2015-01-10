#ifndef EERTOSHAL_H
#define EERTOSHAL_H

#include "RTOS/HAL.h"
#include <mega128.h>
#include <stdint.h>
#include <stdlib.h>    // itoa

#define STATUS_REG 			    SREG
#define Interrupt_Flag		    SREG_I
#define _disable_interrupts()	#asm("cli")
#define _enable_interrupts()	#asm("sei")
#define wdt_reset()             #asm("wdr")
//RTOS Config

#ifdef __CODEVISIONAVR__
    #define RTOS_ISR  		TIM2_COMP
    #define DEAD_TIME_ISR   TIM0_COMP
 #else
    #define RTOS_ISR  		TIMER2_COMP_vect   
    #define DEAD_TIME_ISR   TIMER0_COMP_vect  
#endif

#define MainTimerQueueSize	20

extern void RTOS_timer_init (void);
extern void RunRTOS (void);      //запуск системмного таймера
extern void StopRTOS (void);     //замедление системмного таймера
extern void FullStopRTOS (void); //полная остановка системмного таймера

extern void DeadTimerInit (void);
extern void DeadTimerRun (void);
extern void DeadTimerStop (void);
#endif
