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
  выполняются задачи из очереди TTask, которые уже "выщелкали",
  соответственно количество кода очень уменьшилось.
  MEMORY -58 WORDS
*/

  //постановка задачи в очередь - 290мкс
  // SetTimerTask + TaskManager для 30 задач = 312мкс (по замерам в протеусе)

// Очереди задач, таймеров.
// Тип данных - указатель на функцию
//volatile static TPTR	TaskQueue[TaskQueueSize+1];			// очередь указателей
//update
enum TASK_STATUS {WAIT, RDY, IN_PROC, DONE};
#warning оптимизировать передачей указателя или ссылки а структуру

typedef  struct 
{
                        TPTR GoToTask; 					// Указатель перехода
                        uint16_t TaskDelay;				// Выдержка в мс перед старотом задачи    
                        uint16_t TaskPeriod;			// Выдержка в мс перед следующим запуском 
                        uint8_t TaskStatus; 
						//TODO добавить параметр и отладить
 #ifdef DEBUG  
  uint32_t sys_tick_time; // Значение системного таймера на момент выполнения задачи в тиках
  uint8_t exec_time;       // Реально замеряное время выполнения задачи
  uint8_t  flag;             // Различные флаги (переполнение таймера, ошибка,..) 
  uint8_t hndl;
 #endif
}TASK_STRUCT;// Структура программного таймера-задачи

 volatile static TASK_STRUCT  TTask[MainTimerQueueSize+1];	// Очередь таймеров
 volatile static uint8_t timers_cnt_tail = 1;
 volatile uint32_t exec_task_addr = 0; //for debug
          
 
  void clear_duplicates (void); //not tested    
  void SheikerSort(uint8_t *a, int n);
  
 //===================================================================
// RTOS Подготовка. Очистка очередей
  void InitRTOS(void)
{
uint8_t	index;
      for(index=0;index!=MainTimerQueueSize+1;index++) // Обнуляем все таймеры.
    {
	    TTask[index].GoToTask = Idle;
	    TTask[index].TaskDelay = 0; //Считаю, что достаточно занулить указатель
	 }
}


//Пустая процедура - простой ядра.
  void  Idle(void)
{ //#warning наполнить полезным функционалом 
#ifdef DEBUG

LED_PORT  &=~(1<<LED1);   //Для отслеживания загрузки системы   
//if(v_u32_SYS_TICK%2600){KERNEL_Sort_TaskQueue();};
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
  for(index=0;index!=timers_cnt_tail;++index)	// ищем любой пустой таймер
  {  
        if (TTask[index].GoToTask == TS)
	{
		TTask[index].TaskDelay = NewTime;		// И поле выдержки времени    
        TTask[index].TaskPeriod = NewPeriod;	// И поле периода запуска
        TTask[index].TaskStatus = WAIT;      //Флаг - ожидает выполнения!
        if (nointerrupted) {_enable_interrupts();}	// Разрешаем прерывания
        return;									// Выход.
	}
	if (TTask[index].GoToTask == Idle)
	{
		TTask[index].GoToTask = TS;			// Заполняем поле перехода задачи
		TTask[index].TaskDelay = NewTime;		// И поле выдержки времени        
        TTask[index].TaskPeriod = NewPeriod;	// И поле периода запуска
        TTask[index].TaskStatus = WAIT;      //Флаг - ожидает выполнения!
                                  //подсчёт исспользованых таймеров
        timers_cnt_tail++; //чтобы не прочёсывать пустую часть очереди
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
char tmp_str[10];
bit task_exist = 1;// существует ли задача всё ещё

TPTR task;                 //TODO сделать глобальными регистровыми!

  for(index=0;index!=timers_cnt_tail;++index)   // Прочесываем очередь в поисках нужной задачи
	{	
      if ((TTask[index].TaskStatus == RDY)) // пропускаем пустые задачи и те, время которых еще не подошло
		{
          LED_PORT |= (1<<LED1);   //Для отслеживания загрузки системы
          task=TTask[index].GoToTask;  // запомним задачу т.к. во время выполнения может измениться индекс
              
            
          if(TTask[index].TaskPeriod == 0) //если период 0 - удаляем задачу из списка
           {
                ClearTimerTask(task);  task_exist = 0;// задачи больше не существует
           } 
           else 
           {
                TTask[index].TaskDelay = TTask[index].TaskPeriod; //перезапись задержки
                TTask[index].TaskStatus = WAIT;
           }     
             //Дальше идём на выполнение задачи
     
#ifdef DEBUG                                            //запись св-ств задачи для лога
	_disable_interrupts();            
          if(task_exist){ TTask[index].sys_tick_time = v_u32_SYS_TICK;}  //если задача не удалилась                                    
	_enable_interrupts();
#endif  
             v_u8_SYS_TICK_TMP1 = (uint8_t)v_u32_SYS_TICK; //засекаем время віполнения задачи
            _enable_interrupts();						// Разрешаем прерывания
            (task)();								    // Переходим к задаче    
            if(task_exist){  //если задача не удалилась  
             if((v_u8_SYS_TICK_TMP1 = (uint8_t)v_u32_SYS_TICK - v_u8_SYS_TICK_TMP1)>=1){itoa(v_u8_SYS_TICK_TMP1,tmp_str);Put_In_Log(tmp_str);Put_In_Log("%\r");}
              TTask[index].exec_time = v_u8_SYS_TICK_TMP1;//то запишем время её выполнения
             }
            //всё стопорится на єтой строке если не return!!!
           // TTask[index].TaskStatus = DONE;         //теперь просто меняем статус
/*#ifdef DEBUG
	_disable_interrupts();            
            Timer_3_stop();                            //выключили отсчёт времени выполнения
            TTask[index].exec_time = Timer_3_get_val();
            TTask[index].flag = v_u16_TIM_1_OVR_FLAG;
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

for(index=0;index!=timers_cnt_tail;index++)		// Прочесываем очередь таймеров
	{
         if( TTask[index].TaskStatus == WAIT) 		// Если не выполнилась и
           {
             if(TTask[index].TaskDelay > 0)  // таймер не выщелкал, то
              {					
                TTask[index].TaskDelay--;	// щелкаем еще раз.   
              }  
              else                         //Ставим флаг готовности к выполнению
              {
               TTask[index].TaskStatus = RDY;
              }      
		};
	}
}

void ClearTimerTask(TPTR TS)  //обнуление таймера
{
uint8_t	 index=0;
bit nointerrupted = 0;
if (STATUS_REG & (1<<Interrupt_Flag))
{
_disable_interrupts(); nointerrupted = 1;
}
    for(index=0; index<timers_cnt_tail; ++index)
    {
      if(TTask[index].GoToTask == TS)
      {                                         
           if(index != (timers_cnt_tail - 1))         // переносим последнюю задачу
         {                                            // на место удаляемой
            TTask[index] = TTask[timers_cnt_tail - 1];      
                         //зануление последней задачи
            TTask[timers_cnt_tail - 1].GoToTask = Idle;
            TTask[timers_cnt_tail - 1].TaskDelay = 0; // Обнуляем время      
            TTask[timers_cnt_tail - 1].TaskPeriod = 0; // Обнуляем время     
            TTask[timers_cnt_tail - 1].TaskStatus = DONE; // Обнуляем status
         }
           else
         {
             //Если задача последняя в очереди
            TTask[index].GoToTask = Idle;
            TTask[index].TaskDelay = 0; // Обнуляем время      
            TTask[index].TaskPeriod = 0; // Обнуляем время  
            TTask[index].TaskStatus = DONE; // Обнуляем status
         }  
        timers_cnt_tail--;  //уменьшаем кол-во задач     
        if (nointerrupted) _enable_interrupts();
        return;
      }
    }
}


  void KERNEL_Sort_TaskQueue (void) //сортировкa задач по периоду выполнения (наиболее частые - ближе к началу очереди!)  
 {
  bit		nointerrupted = 0;
  int8_t l, r, k, index;       
  TASK_STRUCT tmp;        
  
 if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания	
  //+++++++++++++      
           k = l = 0;
           r = timers_cnt_tail - 3; //In original = 2!            
           while(l <= r)
           {
              for(index = l; index <= r; index++)
              {
                 if (TTask[index].TaskPeriod > TTask[index+1].TaskPeriod)
                 {   
                 tmp = TTask[index];
                 TTask[index] = TTask[index+1];
                 TTask[index+1] = tmp;
                    k = index;
                 }
              }
              r = k - 1;

              for(index = r; index >= l; index--)
               {
                 if (TTask[index].TaskPeriod > TTask[index+1].TaskPeriod)
                 {   
                 tmp = TTask[index];
                 TTask[index] = TTask[index+1];
                 TTask[index+1] = tmp;
                    k = index;
                 }
               }
              l = k + 1;
           }           
 //-------------
  if (nointerrupted){_enable_interrupts();}	// Разрешаем прерывания     
 }
 
 

  //TODO look at http://we.easyelectronics.ru/Soft/minimalistichnaya-ochered-zadach-na-c.html
 //TODO look at http://we.easyelectronics.ru/Soft/dispetcher-snova-dispetcher.html
 
 void clear_duplicates (void) //not tested
 {
  uint8_t		index=0;
  bit		nointerrupted = 0;
  TPTR task_src;
if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания	
  for(index=0;index!=timers_cnt_tail;++index)	
  {       
     task_src = TTask[index].GoToTask;
    for(index+1;index!=timers_cnt_tail;++index)	
      { 
       if (TTask[index].GoToTask == task_src) {TTask[index].GoToTask = Idle;}
      } 
  }
  if (nointerrupted){_enable_interrupts();}	// Разрешаем прерывания     
 }
 

void Task_t_props_out (void)
{ 
uint8_t index = 0;
char tmp_str[10];

 FullStopRTOS();
    // LED_PORT  &=~(1<<LED2);
  for(index=0;index!=timers_cnt_tail;++index)	// ищем таймер
	{         
     Put_In_Log("\r\n<");    
     itoa((int)TTask[index].GoToTask , tmp_str);
     Put_In_Log(tmp_str); Put_In_Log(",");
     itoa((int)TTask[index].TaskDelay , tmp_str);
     Put_In_Log(tmp_str); Put_In_Log(","); 
     itoa((int)TTask[index].TaskPeriod , tmp_str);
     Put_In_Log(tmp_str); Put_In_Log(",");
     itoa((int)TTask[index].sys_tick_time , tmp_str);      
     Put_In_Log(tmp_str); Put_In_Log(",");
     itoa((int)TTask[index].exec_time , tmp_str);      
     Put_In_Log(tmp_str);Put_In_Log(",");     
     itoa((int)TTask[index].TaskStatus , tmp_str);  
     Put_In_Log(tmp_str); 
     Put_In_Log(">");       
  }    
 // LED_PORT  |=(1<<LED2); 
 Put_In_Log("\r\n"); 
 #asm("sei");
 Task_LogOut();  
 RunRTOS();
}

/*
TPTR GoToTask; 					// Указатель перехода
uint16_t TaskDelay;				// Выдержка в мс перед старотом задачи    
uint16_t TaskPeriod;			// Выдержка в мс перед следующим запуском 
uint8_t TaskStatus; 
  uint32_t sys_tick_time; // Значение системного таймера на момент выполнения задачи в тиках
  uint8_t exec_time;       // Реально замеряное время выполнения задачи
  uint8_t hndl;
*/

//TPTR> Delay> Period> tick> exec Status>

/*2400
<3720,8,10,2404,0,0>
<4295,0,0,0,0,3>
<3759,0,33,0,0,1>
<3674,107,500,2013,46,0>
<3835,48,50,2404,1,0>
<3882,33,333,2106,0,0>
<3666,0,10,2363,0,1>
<3673,0,250,2148,1,1>
<3663,97,0,0,0,0>
<3727,0,3,2363,41,1>
+ */

/*2700 
<4295,0,0,0,0,3>
<3674,298,500,2533,44,0>
<3727,0,3,2698,37,1>
<3720,0,10,2578,0,1>
<3666,0,10,2580,0,1>
<3759,0,33,0,0,1>
<3835,0,50,2578,1,1>
<3673,0,250,2406,0,1>
<3882,47,333,2449,0,0>
+*/
/* 
<4295,0,0,0,0,3>
<3674,294,500,2534,45,0>
<3727,0,3,2703,37,1>
<3720,0,10,2624,0,1>
<3666,0,10,2624,0,1>
<3759,0,33,0,0,1>
<3835,0,50,2580,1,1>
<3673,0,250,2406,0,1>
<3882,42,333,2449,0,0>
+*/