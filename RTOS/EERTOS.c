#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"


//#undef DEBUG
/*
-Насколько хорошо это будет работать, если таймер установить на 50-100мкс при 16МГц?

DI HALT: 9 Апрель 2012 в 21:08
Плохо будет работать. Там довольно многое можно оптимизировать,
среднее выполнение блока таймерной службы сейчас около 600 тактов.
Ну или около того. Столько же в среднем занимает перебор очереди,
установка таймера порядка 700 тактов, установка задачи около 400 тактов.
//=150uS - время на предподготовку задачи (примерное из предыдущ. днн^)
Реально тайминг снизить до 500мкс ,но делать очень короткие задачи, иначе таймер лажать будет.
*/


/*
  UPDATE - Избавился от очереди задач TaskQueue, вместо этого в диспетчере задач
  выполняются задачи из очереди MainTimer, которые уже "выщелкали",
  соответственно количество кода очень уменьшилось.
  MEMORY -58 WORDS
*/

  //постановка задачи в очередь - 290мкс
  // SetTimerTask + TaskManager для 30 задач = 312мкс (по замерам в протеусе)

// Очереди задач, таймеров.
// Тип данных - указатель на функцию
//volatile static TPTR	TaskQueue[TaskQueueSize+1];			// очередь указателей
//update
enum TASK_STATUS {WAIT, RDY,IN_PROC, DONE};
#warning оптимизировать передачей указателя или ссылки а структуру
volatile static struct
{
                        TPTR GoToTask; 					// Указатель перехода
                        uint16_t TaskDelay;				// Выдержка в мс перед старотом задачи    
                        uint16_t TaskPeriod;			// Выдержка в мс перед следующим запуском 
                        uint8_t Task_status; 
						//TODO добавить параметр и отладить
 #ifdef DEBUG  
  uint32_t sys_tick_time; // Значение системного таймера на момент выполнения задачи в тиках
  uint16_t exec_time;       // Реально замеряное время выполнения задачи
  uint8_t  flag;             // Различные флаги (переполнение таймера, ошибка,..) 
  uint8_t hndl;
 #endif
} MainTimer[MainTimerQueueSize+1];	// Очередь таймеров

 volatile uint32_t exec_task_addr = 0; //for debug
 volatile static uint8_t timers_cnt_search_lim = 1;


 //===================================================================
// RTOS Подготовка. Очистка очередей
  void InitRTOS(void)
{
uint8_t	index;
      for(index=0;index!=MainTimerQueueSize+1;index++) // Обнуляем все таймеры.
    {
	    MainTimer[index].GoToTask = Idle;
	    MainTimer[index].TaskDelay = 0;
	 }
}


//Пустая процедура - простой ядра.
  void  Idle(void)
{ //#warning наполнить полезным функционалом 
#ifdef DEBUG

LED_PORT  &=~(1<<LED1);   //Для отслеживания загрузки системы   

#endif
}

 //UPDATE
 void SetTask(TPTR TS){  // Поставить задачу в очередь для немедленного выполнения
 SetTimerTask(TS,0,0);
}


//Функция установки задачи по таймеру. Передаваемые параметры - указатель на функцию,
// Время выдержки в тиках системного таймера. Возвращет код ошибки.

void SetTimerTask(TPTR TS, unsigned int NewTime, unsigned int NewPeriod)    //1 task ~12words
{
uint8_t		index=0;
static uint8_t		timers_cnt=0;
uint8_t		nointerrupted = 0;


if (STATUS_REG & (1<<Interrupt_Flag)) 			// Проверка запрета прерывания, аналогично функции выше
	{
	_disable_interrupts();
	nointerrupted = 1;
	}
// My UPDATE - optimized
  for(index=0;index!=timers_cnt_search_lim;++index)	// ищем любой пустой таймер
  {  
        if (MainTimer[index].GoToTask == TS)
	{
		MainTimer[index].TaskDelay = NewTime;		// И поле выдержки времени    
        MainTimer[index].TaskPeriod = NewPeriod;	// И поле периода запуска
        MainTimer[index].Task_status = WAIT;      //Флаг - ожидает выполнения!
        if (nointerrupted) {_enable_interrupts();}	// Разрешаем прерывания
        return;									// Выход.
	}
  
	if (MainTimer[index].GoToTask == Idle)
	{
		MainTimer[index].GoToTask = TS;			// Заполняем поле перехода задачи
		MainTimer[index].TaskDelay = NewTime;		// И поле выдержки времени        
        MainTimer[index].TaskPeriod = NewPeriod;	// И поле периода запуска
        MainTimer[index].Task_status = WAIT;      //Флаг - ожидает выполнения!
                                  //подсчёт исспользованых таймеров
        timers_cnt_search_lim++; //чтобы не прочёсывать пустую часть очереди
        if (nointerrupted) {_enable_interrupts();}	// Разрешаем прерывания
        return;									// Выход.
	}
  }
// тут можно сделать return c кодом ошибки - нет свободных таймеров
}

/*=================================================================================
Диспетчер задач ОС. Выбирает из очереди задачи и отправляет на выполнение.
*/
volatile uint8_t	tmp_adr;
inline void TaskManager(void)
{
uint8_t		index=0;

TPTR task;                 //TODO сделать глобальными регистровыми!

  for(index=0;index!=timers_cnt_search_lim;++index)   // Прочесываем очередь в поисках нужной задачи
	{	if ((MainTimer[index].Task_status == RDY)) // пропускаем пустые задачи и те, время которых еще не подошло
		{
          LED_PORT |= (1<<LED1);   //Для отслеживания загрузки системы
            task=MainTimer[index].GoToTask;             // запомним задачу т.к. во время выполнения может измениться индекс
#ifdef DEBUG                                            //запись св-ств задачи для лога
	_disable_interrupts();            
            tmp_adr =(uint8_t)task;
            MainTimer[index].hndl = tmp_adr;
            MainTimer[index].sys_tick_time = v_u32_SYS_TICK;
           // Timer_3_start();                           //включили отсчёт времени выполнения
	_enable_interrupts();
#endif
		    //MainTimer[index].GoToTask = Idle;           // ставим затычку   
            MainTimer[index].Task_status = IN_PROC;         //теперь просто меняем статус
            _enable_interrupts();						// Разрешаем прерывания
            (task)();								    // Переходим к задаче     
            //всё стопорится на єтой строке если не return!!!
           // MainTimer[index].Task_status = DONE;         //теперь просто меняем статус
/*#ifdef DEBUG
	_disable_interrupts();            
            Timer_3_stop();                            //выключили отсчёт времени выполнения
            MainTimer[index].exec_time = Timer_3_get_val();
            MainTimer[index].flag = v_u16_TIM_1_OVR_FLAG;
    _enable_interrupts();
#endif*/
            return;                                     // выход до следующего цикла
		}
	}
    _enable_interrupts();							// Разрешаем прерывания
	Idle();                                     // обошли задачи, нужных нет - простой
}

/*
Служба таймеров ядра. Должна вызываться из прерывания раз в 1мс. Хотя время можно варьировать в зависимости от задачи

To DO: Привести к возможности загружать произвольную очередь таймеров. Тогда можно будет создавать их целую прорву.
А также использовать эту функцию произвольным образом.
В этом случае не забыть добавить проверку прерывания.
*/
inline void TimerService(void)
{
uint8_t index;

for(index=0;index!=timers_cnt_search_lim;index++)		// Прочесываем очередь таймеров
	{
         if( MainTimer[index].Task_status == WAIT) 		// Если не выполнилась и
           {
             if(MainTimer[index].TaskDelay > 0)  // таймер не выщелкал, то
              {					
                MainTimer[index].TaskDelay--;	// щелкаем еще раз.   
              }  
              else                         //Ставим флаг готовности к віполнению
              {
               MainTimer[index].Task_status = RDY;
              }      
		};
	}
}

void ClearTimerTask(TPTR TS)  //обнуление таймера
{
uint8_t	 index=0;
uint8_t nointerrupted = 0;
if (STATUS_REG & (1<<Interrupt_Flag))
{
_disable_interrupts();
nointerrupted = 1;
}
    for(index=0; index!=timers_cnt_search_lim; ++index)
    {
        if(MainTimer[index].GoToTask == TS)
        {                                         
        /*     //возможно стоит сделать так
           if(i != (arrayTail - 1))                     // переносим последнюю задачу
         {                                            // на место удаляемой
            TaskArray[i] = TaskArray[arrayTail - 1];
         }
        */
            MainTimer[index].GoToTask = Idle;
            MainTimer[index].TaskDelay = 0; // Обнуляем время  
            timers_cnt_search_lim--;  //уменьшаем кол-во задач
            if (nointerrupted) _enable_interrupts();
            return;
        }
    }
}


  //TODO look at http://we.easyelectronics.ru/Soft/minimalistichnaya-ochered-zadach-na-c.html
 //TODO look at http://we.easyelectronics.ru/Soft/dispetcher-snova-dispetcher.html