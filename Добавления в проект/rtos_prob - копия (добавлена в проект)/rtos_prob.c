
#include <mega128.h>
#include <delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <io.h>

#include "RTOS/HAL.h"
#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"

//RTOS Interrupt
// Timer2 overflow interrupt service routine
interrupt [RTOS_ISR] void timer2_comp_isr(void)
{
 //DDRB.4^=1; 
 TimerService();
}

// Прототипы задач ============================================================
void Task1 (void);
void Task2 (void);
void Task3 (void);
void Task4 (void);

void Task5 (void);
void Task6 (void);
//============================================================================
//Область задач
//============================================================================

void Task1 (void)     
{
SetTimerTask(Task2,500);
LED_PORT  |=1<<LED1;
}

void Task2 (void)
{
SetTimerTask(Task1,500);
LED_PORT  &= ~(1<<LED1);
}

void Task3 (void)
{
SetTimerTask(Task4,1000);
LED_PORT  |= (1<<LED2);
}
void Task4 (void)
{
SetTimerTask(Task3,1000);
LED_PORT  &=~ (1<<LED2);
}





// Declare your global variables here

void main(void)
{
InitAll();			// Инициализируем периферию
InitRTOS();			// Инициализируем ядро
RunRTOS();			// Старт ядра. 

// Запуск фоновых задач.
SetTask(Task1);     //290uS (50/50) and (10/10)   но при 1/1 таск 1 лагает 
SetTask(Task3);    //290us  
 
  //SetTask(Task5);    //при добавлении задачи 285us высок ур 11мс, а низк - 2.5(!)

while(1) 		// Главный цикл диспетчера
{
//wdt_reset();	// Сброс собачьего таймера
TaskManager();	// Вызов диспетчера
}
}
//290uS (50/50) and (10/10)   но при 1/1 таск 1 лагает:
// (1 мс низк 10.1 высок волнистый и инверсия фал таск 1 и 2)
//при кол-ве задач  6 время задержки выполнения каждой по прежнему ~285uS
                                                                       