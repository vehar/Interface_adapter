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

void main(void)
{
char i;
char str[7];

HARDWARE_init();
SOFTWARE_init();

#ifdef DEBUG
    //DDRD.7=1;//PORTD.7=1;  //Led VD2
    //DDRD.6=1;PORTD.6=1;    //Led VD1
    USART_SendStrFl(USART_1,start);
    USART_SendStrFl(SYSTEM_USART,start);
#endif


//#asm("sei") // Global enable interrupts Upd-1
//sprintf(lcd_buf, "Z=%d", SYS_TICK); ;LcdString(1,3); LcdUpdate();
RunRTOS();			// Старт ядра.

//delay_ms(1000);// Запуск фоновых задач.
SetTask(Task_Start);     //290uS (50/50) and (10/10) но при 1/1 таск 1 лагает

///-----------------Upd-7-----------------------------
// первичный запуск всех задач
SetTimerTask(Task_pars_cmd, 25); //Upd-6
#ifdef DEBUG                    //Upd-6
SetTimerTask(Task_LogOut,50);
SetTimerTask(Task_ADC_test,5000);   //Upd-6
SetTask(Task_LcdGreetImage);    //Upd-4
SetTask(Task_AdcOnLcd);
#endif
///---------------------------------------------------


delay_ms(1000); //?
while (1)
 {
//wdt_reset();	// Сброс собачьего таймера
TaskManager();	// Вызов диспетчера
 }
} //END MAIN



// Timer2 interrupt service routine
interrupt [RTOS_ISR] void timer2_comp_isr(void)//RTOS Interrupt 1mS
{
 TimerService();
 SYS_TICK++;
}