                       /*****************************************************
Project : Uni_interface_adapter
Version :
Date    : 04.02.2014
Author  : Vlad

Chip type               : ATmega128
Program type            : Application
AVR Core Clock frequency: 16,000000 MHz
Data Stack size         : 1024
*****************************************************/

#define DEBUG

#include <adapter.h>
#include "init.c"
#include "D_Globals/global_variables.h"
#include "D_Globals/global_defines.h"
#include "PARSER/pars_hndl.c"           //Upd-8 in folder

// G_vars are here
volatile uint16_t tmp = 0;

inline void ProcessMessages(void);//Обработчик флагов и сообщений задач

//TODO переписать парсер команд в конечный автомат как в http://habrahabr.ru/post/241941/
 //Предусмотреть убийство задачи, если войдёт в бесконечный цикл! (2-й таймер)
  //Добавить вытесняемость!
 //Отладить прерывание SPI!  
 //Добавить сортировку задач по периоду выполнения (наиболее частые - ближе к началу очереди!)+  
  //TODO Теперь надо передать управление системе после удаления из очереди зависшей задачи!
  
//===================================================================================
//=================================================================================== 
//START MAIN



void main(void)
{ 
#ifdef DEBUG   //синхронизация с протеусом
    DDRD.LED2=1;LED_PORT |= (1<<LED2);  //Led VD2
    delay_ms(15);
    DDRD.LED2=0;LED_PORT  &= ~(1<<LED2); //Led VD2
#endif

HARDWARE_init();
SOFTWARE_init();
//DeadTimerInit();
InitRTOS();

#ifdef DEBUG
    DDRD.LED2=1;//PORTD.7=1;  //Led VD2
    DDRD.LED1=1;//PORTD.6=1;    //Led VD1
    DDRD.LED3=1;//PORTD.5=1;    //Led LED3
    USART_Send_StrFl(USART_1,start);
    USART_Send_StrFl(SYSTEM_USART,start);
#endif

//RunRTOS();			// Старт ядра.

// Запуск фоновых задач.
SetTask(Task_Initial);

RunRTOS();			// Старт ядра.

while (1)
 {
//wdt_reset();	// Сброс собачьего таймера
TaskManager();	// Вызов диспетчера
ProcessMessages();//Обработка различных флагов и сообщений от задач
 }
} //END MAIN
//===================================================================================
//===================================================================================






inline void ProcessMessages(void)
{
Task_FlagsHandler();
}  



// Timer2 interrupt service routine
interrupt [RTOS_ISR] void timer2_comp_isr(void)//RTOS Interrupt 1mS
{
//static uint16_t tmp_tick;

 v_u32_SYS_TICK++;  
// if(v_u32_SYS_TICK%10 == 0)
// {
  TimerService();
// }
 
  CorpseService(); //очистка от зависших задач

}




interrupt [DEAD_TIME_ISR] void timer0_comp_isr(void)//DEAD_TIME_ISR Interrupt 
{
  LED_PORT.LED3^=1;
}







// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
v_u16_TIM_1_OVR_FLAG++;
}


//sprintf(lcd_buf, "Z=%d", v_u32_SYS_TICK); ;LcdString(1,3); LcdUpdate();