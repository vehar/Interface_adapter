#include "D_Globals/global_defines.h"     //Upd-8 in folder
#include "D_Globals/global_variables.h"   //Upd-8 in folder
#include "D_usart/usart.h"
#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"
#include "D_Tasks/task_list.h"

#define LogBufSize 512 //Размер буффера для логов

/////// Отладочный кусок.
//Вывод лога работы конечного автомата в буфер памяти, а потом.
//По окончании работы через UART на волю

static volatile char WorkLog[LogBufSize];
static volatile uint16_t LogIndex = 0;

void WorkLogPutChar(unsigned char symbol){
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
  strcat(data,"\r\n"); //not tested!
  while(*data)
  {
    WorkLogPutChar(*data++);
  }
}

void LogOut(void)				// Выброс логов
{
StopRTOS();
WorkLog[LogIndex]= 0xFF;
LogIndex++;
USART_Send_Str(USART_0, WorkLog);
RunRTOS();

SetTimerTask(Task_Flush_WorkLog,10);//очистка лог буффера
LogIndex = 0;
}
/////////////

#define _Led1_ON LED_PORT|=(1<<LED1)
#define _Led1_OFF LED_PORT&=~(1<<LED1)