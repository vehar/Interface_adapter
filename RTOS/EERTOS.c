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


  //постановка задачи в очередь - 290мкс
  // SetTimerTask + TaskManager для 30 задач = 312мкс (по замерам в протеусе)




enum TASK_STATUS {WAIT, RDY, IN_PROC, DONE, DEAD};

enum SetTimerTask_Status {QUEUE_FULL, TASK_REWRITTEN, TASK_ADDED, DEAD_TASK};

#warning оптимизировать передачей указателя или ссылки на структуру
typedef  struct 
{
                        TPTR GoToTask; 					// Указатель перехода
                        uint16_t TaskDelay;				// Выдержка в мс перед старотом задачи    
                        uint16_t TaskPeriod;			// Выдержка в мс перед следующим запуском 
                        uint8_t TaskStatus; 
						//TODO добавить параметр и отладить
 #ifdef DEBUG  
  uint32_t sys_tick_time;  // Значение системного таймера на момент запуска задачи в тиках
  uint8_t exec_time;       // Реально замеряное время выполнения задачи
  //uint8_t  flag;           // Различные флаги (переполнение таймера, ошибка,..) 
 #endif
}TASK_STRUCT;// Структура программного таймера-задачи

#define QUEUE_SORTING_PERIOD 100 //ticks(!)  //можно увеличить, чтоб не грузить Idle-задачу
volatile static uint16_t DeadTaskTimeout = 10;

 volatile static TASK_STRUCT  TTask[MainTimerQueueSize+1];	// Очередь таймеров
 volatile static uint8_t timers_cnt_tail = 1; 
 volatile bit InfiniteLoopFlag = 1; //Если задача зависнет - то в прерывании об этом узнают и прибьют по таймауту!
        
		
//+++++++++++++PRIVATE RTOS SERVICES++++++++++++++++++++++++++++++
  void KERNEL_Sort_TaskQueue (void); //Сортировка задач по периоду (выполняется фоном)
  void clear_duplicates (void); //not tested    
  inline void dbg_out (char index);
  void SheikerSort(uint8_t *a, int n);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  
//===============================================================================================
  void  Idle(void)  //Пустая процедура - простой ядра.
{ 
	#ifdef DEBUG
	LED_PORT  &=~(1<<LED1);   //Для отслеживания загрузки системы   
	#endif  
	if(v_u32_SYS_TICK % QUEUE_SORTING_PERIOD == 0){KERNEL_Sort_TaskQueue();}; //Периодическое упорядочевание задач по длительости переиода
}
//===============================================================================================
  
  
//=============================================================================================== 
  void InitRTOS(void) // RTOS Подготовка. Очистка очередей
{
uint8_t	index;
RTOS_timer_init(); //Hardware!

      for(index=0;index!=MainTimerQueueSize+1;index++) // Обнуляем все таймеры.
    {
	    TTask[index].GoToTask = Idle;
	    TTask[index].TaskDelay = 0; //Считаю, что достаточно занулить указатель
	 }
}
//===============================================================================================


//===============================================================================================
 void SetTask(TPTR TS)  // Поставить задачу в очередь для немедленного выполнения
{
 SetTimerTask(TS,0,0);
}
//===============================================================================================



//Функция установки задачи по таймеру. Передаваемые параметры - указатель на функцию,
// Время выдержки в тиках системного таймера. Возвращет код ошибки.
//===============================================================================================
uint8_t SetTimerTask(TPTR TS, unsigned int NewTime, unsigned int NewPeriod)    //1 task ~12words
{
uint8_t		index=0;
uint8_t		result = QUEUE_FULL;
bit			nointerrupted = 0;

if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания

  for(index=0;index!=timers_cnt_tail;++index)	
  {  
	if (TTask[index].GoToTask == TS)			// ищем заданый таймер
	{
		if(TTask[index].TaskStatus != DEAD)				// Если задача не помечена как мёртвая(зависшая) утилитой CorpseService()
		{
			TTask[index].TaskDelay  = NewTime;		    // И поле выдержки времени    
			TTask[index].TaskPeriod = NewPeriod;	    // И поле периода запуска
			TTask[index].TaskStatus = WAIT;             // Флаг - ожидает выполнения!
			result = TASK_REWRITTEN; 
			goto exit;			// Выход.
		}	else{result = DEAD_TASK; goto exit;}		//Устанавливать на выполнение висячие задачи нельзя!											
	  }	  
  } 
    // Если не находим - значит он новый
	if(timers_cnt_tail < MainTimerQueueSize)// И в очереди есть место - добавляем задачу в конец очереди
	{
			TTask[timers_cnt_tail].GoToTask   = TS;			    // Заполняем поле перехода задачи
			TTask[timers_cnt_tail].TaskDelay  = NewTime;		// И поле выдержки времени        
			TTask[timers_cnt_tail].TaskPeriod = NewPeriod;	    // И поле периода запуска
			TTask[timers_cnt_tail].TaskStatus = WAIT;           // Флаг - ожидает выполнения!
			timers_cnt_tail++;                          		// Увеличиваем кол-во (новых) таймеров			
			result = TASK_ADDED; 
			goto exit;			    							// Выход.
	}		
  
exit:  
  if (nointerrupted) {_enable_interrupts();}			// Разрешаем прерывания    
  return result; // return c кодом ошибки - нет свободных таймеров, таймер перезаписан или добавлен как новый	
}
//===============================================================================================


/*
Служба таймеров ядра. Должна вызываться из прерывания раз в 1мс. Хотя время можно варьировать в зависимости от задачи

TODO: Привести к возможности загружать произвольную очередь таймеров. Тогда можно будет создавать их целую прорву.
А также использовать эту функцию произвольным образом.В этом случае не забыть добавить проверку прерывания.
*/
//===============================================================================================
inline void TimerService(void)
{
uint8_t index;

for(index=0;index!=timers_cnt_tail;index++)		// Прочесываем очередь таймеров
	{
         if((TTask[index].TaskStatus == WAIT) || (TTask[index].TaskStatus == DONE))// Если не выполнилась или выполнилась
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


//===============================================================================================
//Диспетчер задач ОС. Выбирает из очереди задачи и отправляет на выполнение
//===============================================================================================
inline void TaskManager(void) //в диспетчере задач выполняются задачи из очереди TTask, которые уже "выщелкали"
{
uint8_t		index=0;
char 		tmp_str[10];
bit 		task_exist = 1;// существует ли задача всё ещё
TPTR 		CurrentTask;               

  for(index=0;index!=timers_cnt_tail;++index)   // Прочесываем очередь задач
	{	
      if ((TTask[index].TaskStatus == RDY)) // Если задача отмечена к выполнению (пропускаем пустые и те, время которых еще не подошло)
		{
          LED_PORT |= (1<<LED1);   //Для отслеживания загрузки системы
          CurrentTask=TTask[index].GoToTask;  // запомним задачу т.к. во время выполнения может измениться индекс

           if(TTask[index].TaskPeriod == 0) //если период 0 - удаляем задачу из списка
           {
                ClearTimerTask(CurrentTask);  task_exist = 0;// задачи больше не существует
           } 
           else 
           {
                TTask[index].TaskDelay = TTask[index].TaskPeriod; //перезапись задержки
                TTask[index].TaskStatus = IN_PROC;  //Задача в процессе выполнения     
#ifdef DEBUG                                            //запись св-ств задачи для лога
                TTask[index].sys_tick_time = v_u32_SYS_TICK; //время начала выполнения                                  
#endif       
           }     
             //Дальше идём на выполнение задачи
 //----------------------------------------------------------------------------------------------------    
            v_u8_SYS_TICK_TMP1 = (uint8_t)v_u32_SYS_TICK; //засекаем время выполнения задачи
            _enable_interrupts();						// Разрешаем прерывания
            (CurrentTask)();					        // ПЕРЕХОД К ЗАДАЧЕ!    
            
InfiniteLoopFlag = 0; //Если задача зависнет - то в прерывании об этом узнают и прибьют по таймауту!
 //----------------------------------------------------------------------------------------------------
		
            if(task_exist)//если задача ранее не удалилась  
			{  
			   if(TTask[index].TaskStatus != DEAD) //Если задача не была отмечена в прерывании как зависшая
			   {
				TTask[index].TaskStatus = DONE; //меняем статус - благополучно выполнилась! 
			   }      
				v_u8_SYS_TICK_TMP1 = (uint8_t)v_u32_SYS_TICK - v_u8_SYS_TICK_TMP1;   
                TTask[index].exec_time = v_u8_SYS_TICK_TMP1;//запишем время её выполнения
#ifdef DEBUG 				
				 if(v_u8_SYS_TICK_TMP1)
				 {
					itoa(v_u8_SYS_TICK_TMP1,tmp_str);
					Put_In_Log(tmp_str);Put_In_Log("%\r");
				 }
#endif  			 
             }  

            //всё стопорится на єтой строке если не return!!!
            return;                                     // выход до следующего цикла
		}
	}
    _enable_interrupts();							// Разрешаем прерывания
	Idle();  // обошли задачи, нужных нет - простой, выполнение фоновых служб, сон до следующего прерывания таймера...
}
//===============================================================================================


//===============================================================================================
void ClearTimerTask(TPTR TS)  //обнуление таймера, очистка задачи
{
uint8_t	 	index=0;
bit 		nointerrupted = 0;

if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts(); nointerrupted = 1;}

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
         
        --timers_cnt_tail;  //уменьшаем кол-во задач     
        if (nointerrupted){ _enable_interrupts();}
        return;
      }
    }
}
//===============================================================================================


//===============================================================================================
  void KERNEL_Sort_TaskQueue (void) //сортировкa задач по периоду выполнения (наиболее частые - ближе к началу очереди!)  
 {     
  TASK_STRUCT 	tmp;        
  int8_t 		l, r, k, index;   
  bit			nointerrupted = 0;

 if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания	
 
  //+++++++++++++  //Шейкерная сортировка    
           k = l = 0;
           r = timers_cnt_tail - 2; //        
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
 //===============================================================================================
 
 
 //===============================================================================================
 inline void CorpseService(void) //Обработка зависших задач
{
 static TPTR 		DeadTask_prev, DeadTask_curr;
 static uint16_t 	Timeout_delay = 0;
 static uint8_t 	coins = 0; //совпадения
 uint8_t			index = 0;   
 static bit 		suspect_flag = 0;  
 bit				nointerrupted = 0;
  
  if(InfiniteLoopFlag == 0) //диспетчер сбросил флаг, значит задача завершилась
  {   
	goto EXIT;
  }
  else  //флаг не сброшен - выполняется какая-то задача! возможно уже долго или вообще зависла
  {
	if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания	

	for(index=0; index<timers_cnt_tail; ++index)	//поиск мертвеца (пока ещё просто возможного!)
			 {   
				if(TTask[index].TaskStatus == IN_PROC) //TODO добавить фильтр задач на игнорирование
				{
					if(suspect_flag == 0)//при первом заходе
					{
						DeadTask_prev = TTask[index].GoToTask;
						suspect_flag = 1; //начинаем подозревать
						Timeout_delay = (uint16_t)v_u32_SYS_TICK; //засекли таймаут
						return;
					}
					else //при втором заходе
					{	
						DeadTask_curr = TTask[index].GoToTask;	
						if(DeadTask_curr == DeadTask_prev)	//подозревания подтвердились
							{
							    coins++;
								if((v_u32_SYS_TICK - Timeout_delay >= DeadTaskTimeout)&&(coins>=4))
								{      
                                   TTask[index].TaskStatus = DEAD;	//Поставить метку (обработать в будущем)
								  //ClearTimerTask(DeadTask_curr);	//или просто выпилить из очереди!	
							     
								  InfiniteLoopFlag = 1; //Установка до след. интерации
                                  suspect_flag = 0;Timeout_delay = 0;					
							      DeadTask_curr = DeadTask_prev = 0; coins = 0;  
								  //на goto EXIT; нельзя т.к.
                                  //TODO Теперь надо передать управление системе (пока не реализовано)     
								  
									//FLAG_SET(g_tcf,DEAD_TASK_DELETED);								  
									//TaskManager();
									//#asm("JMP 0x0000");	      
									// #asm("call TaskManager");         
									//goto DEAD_TASK_DETECTED;								  
								}
							}
							else //подозревания НЕ подтвердились (На выполнении сейчас другая задача)
							{
EXIT:							
									InfiniteLoopFlag = 1; //Установка до след. интерации и выход
									suspect_flag = 0;Timeout_delay = 0;					
									DeadTask_curr = DeadTask_prev = 0; coins = 0; 
							}
					
					}
				}
			 }
  }
 if (nointerrupted){_enable_interrupts();}	// Разрешаем прерывания     
}   
//===============================================================================================
 

 //TODO look at http://we.easyelectronics.ru/Soft/minimalistichnaya-ochered-zadach-na-c.html
 //TODO look at http://we.easyelectronics.ru/Soft/dispetcher-snova-dispetcher.html
 
 
 //===============================================================================================
 //===============================================================================================
 //===============================================================================================
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
 //RunRTOS();
}

inline void dbg_out (char index)
{
char tmp_str[10];
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
     Put_In_Log(">\r\n");  
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