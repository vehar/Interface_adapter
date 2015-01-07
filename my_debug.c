//#include "my_debug.h"     //Upd-8 in folder


#include "D_Globals/global_defines.h"     //Upd-8 in folder
#include "D_Globals/global_variables.h"   //Upd-8 in folder
#include "D_usart/usart.h"
#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"
#include "D_Tasks/task_list.h"

#define LogBufSize 512 //Размер буффера для логов
static volatile char WorkLog[LogBufSize];
static volatile uint16_t LogIndex = 0;

/////// Отладочный кусок.
//Вывод лога работы конечного автомата в буфер памяти, а потом.
//По окончании работы через UART на волю

void WorkLogPutChar(unsigned char symbol)
{
__disable_interrupts();
if (LogIndex <LogBufSize)            // Если лог не переполнен
{
        WorkLog[LogIndex]= symbol;    // Пишем статус в лог
        LogIndex++;
}
 __restore_interrupts();
}

void Put_In_LogFl (unsigned char __flash* data){
  while(*data)
  {
    WorkLogPutChar(*data++);
  }
}

//#warning  -добавить после каждого слова прибавление \r\n для єкономии РАМ
void Put_In_Log (unsigned char * data)
{
  while(*data)
  {
    WorkLogPutChar(*data++);
  }
  /// WorkLogPutChar(10);//\r
   //WorkLogPutChar(13);//\n
}

void LogOut(void)				// Выброс логов
{
StopRTOS();

WorkLog[LogIndex]= '+';
LogIndex++;
USART_Send_Str(SYSTEM_USART, WorkLog);

RunRTOS();

FLAG_SET(g_tcf, FLUSH_WORKLOG);//SetTimerTask(Task_Flush_WorkLog,25,0);//очистка лог буффера
LogIndex = 0;
}

void Timer_3_start(void)
{
   TCCR3B =(1<<CS31)|(1<<CS30);//presc = 64
//TIMSK |=0x04; //Tim1 ovrflow Interrupt  ON
}

void Timer_3_stop(void)
{
ICR3H=0x00;     ICR3L=0x00;
OCR3AH=0x00;    OCR3AL=0x00;
OCR3BH=0x00;    OCR3BL=0x00;
OCR3CH=0x00;    OCR3CL=0x00;
TCNT3H=0x00;    TCNT3L=0x00;
TCCR3A=0x00;    TCCR3B==0x00;
//TIMSK &=~0x04; //Tim1 ovrflow Interrupt  OFF
}

uint16_t Timer_3_get_val (void)
{
uint16_t val = 0;
//__disable_interrupts();
 val = TCNT3H;
//__restore_interrupts();
return val;
}
/////////////
