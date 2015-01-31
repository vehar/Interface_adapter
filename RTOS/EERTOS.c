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


//-----------------------------------------------------------------------------------
//Ядро:
//KERNEL_Sort_TaskQueue();  300us //Запускается редко
//TaskManager();            150us
//SetTimerTask();           180us
//Таймерное прерывание 1мс:
//TimerService();           60us
//CorpseService();          180us 
/*
//Кол-во задач =            10;
//Установка 1 задачи =      10 байт
//+
//Сама структура =          13 байт
//= 1_Задача в RAM =        23 байта
ATmega128 memory use summary [bytes]:
Segment   Begin    End      Code   Data   Used    Size   Use%
---------------------------------------------------------------
[.cseg] 0x000000 0x000af4   2626    178   2804  131072   2.1%
[.dseg] 0x000100 0x0005ca      0    202    202    4351   4.6%
*/
//-----------------------------------------------------------------------------------


enum TASK_STATUS {WAIT, RDY, IN_PROC, DONE, DEAD};

enum SetTimerTask_Status {QUEUE_FULL, TASK_REWRITTEN, TASK_ADDED, DEAD_TASK};

#warning оптимизировать передачей указателя или ссылки на структуру
typedef  struct                                         //TODO добавить параметр и отладить 
{
                        TPTR GoToTask; 					// Указатель перехода
                        uint16_t TaskDelay;				// Выдержка в мс перед старотом задачи    
                        uint16_t TaskPeriod;			// Выдержка в мс перед следующим запуском 
                        uint8_t TaskStatus; 						  
#ifdef USE_CORPSE_SERVISE                             
                        uint8_t deadtime;       // Время на выполнение задачи 0 - дефолтный deadtime, 0xFF - бесконечно  
#endif
                        
#ifdef USE_TTASKS_LOGGING
                        uint16_t run_me_cnt;      //кол-во (возможно) упущеных запусков задачи
                        uint16_t exec_time;       // Реально замеряное время выполнения задачи  
                        uint32_t sys_tick_time;  // Значение системного таймера на момент запуска задачи в тиках
#endif
}TASK_STRUCT;// Структура программного таймера-задачи

 volatile static TASK_STRUCT  TTask[TASK_QUEUE_SIZE+1];    // Очередь таймеров    
 volatile static uint8_t timers_cnt_tail = 1; 
 volatile uint32_t v_u32_SYS_TICK; 
  
 #ifdef USE_TTASKS_LOGGING    
 volatile uint8_t v_u8_SYS_TICK_TMP1;    
 #endif         
  
 #ifdef USE_CORPSE_SERVISE 
 volatile bit InfiniteLoopFlag = 1; //Если задача зависнет - то в прерывании об этом узнают и прибьют по таймауту!
 #endif 		
 
 
//+++++++++++++PRIVATE RTOS SERVICES++++++++++++++++++++++++++++++
  void KERNEL_Sort_TaskQueue (void); //Сортировка задач по периоду (выполняется фоном)
  void clear_duplicates (void); //not tested    
  inline void dbg_out (char index);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

bit			global_nointerrupted_flag;
uint8_t      global_interrupt_mask;
uint8_t      global_interrupt_cond;
  
//===============================================================================================
inline void  Idle(void)  //Пустая процедура - простой ядра.
{ 
	#ifdef DEBUG
	_LED1_OFF;   //Для отслеживания загрузки системы   
	#endif  
    
    #ifdef USE_SORTING_TTASK_QUEUE 
	if(v_u32_SYS_TICK % QUEUE_SORTING_PERIOD == 0){KERNEL_Sort_TaskQueue();}; //Периодическое упорядочевание задач по длительости переиода         
    #endif  
   /* ATOMIC_BLOCK_RESTORATE_COND(1)
    {
       _LED1_OFF;
    }  */
}
//===============================================================================================
  
  
//=============================================================================================== 
  void InitRTOS(void) // RTOS Подготовка. Очистка очередей
{
uint8_t	index;
RTOS_timer_init(); //Hardware!

      for(index=0;index!=TASK_QUEUE_SIZE+1;index++) // Обнуляем все таймеры.
    {
	    TTask[index].GoToTask = Idle;
	    TTask[index].TaskDelay = 0; 
		TTask[index].TaskPeriod = 0;
		TTask[index].TaskStatus = WAIT;  
        
#ifdef USE_TTASKS_LOGGING        
		TTask[index].exec_time = 0;
		TTask[index].deadtime = 0;    
        TTask[index].run_me_cnt = 0; 
#endif        
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
uint8_t		index = 0;
uint8_t		result = QUEUE_FULL;
//bit			nointerrupted = 0;
//if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания
 ATOMIC_BLOCK_RESTORESTATE
 {
 // поиск следующей доступной позиции в массиве задач
 //попробовать замену на //while ((TTask[index].GoToTask != TS) && (index < timers_cnt_tail)) {index++; };
   
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
	if(timers_cnt_tail < TASK_QUEUE_SIZE)// И в очереди есть место - добавляем задачу в конец очереди
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
  }//if (nointerrupted) {_enable_interrupts();}			// Разрешаем прерывания            
  return result; // return c кодом ошибки - нет свободных таймеров, таймер перезаписан или добавлен как новый	
}
//===============================================================================================



//===============================================================================================
#ifdef USE_CORPSE_SERVISE
uint8_t SetTaskDeadtime(TPTR TS, uint8_t DeadTime) //DeadTime = 0xFF means DeadTimer (for this task) is OFF, DeadTime = 0x00 use default deadtime
{
uint8_t		index = 0;
uint8_t		result = QUEUE_FULL;
//bit			nointerrupted = 0;

//if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания
 ATOMIC_BLOCK_RESTORESTATE{ 
  for(index=0;index!=timers_cnt_tail;++index)	
  {  
	if (TTask[index].GoToTask == TS)			// ищем заданый таймер
	{
		if(TTask[index].TaskStatus != DEAD)				// Если задача не помечена как мёртвая(зависшая) утилитой CorpseService()
		{
			TTask[index].deadtime  = DeadTime;		    // И поле выдержки времени    
			result = TASK_REWRITTEN; 
			goto exit;			// Выход.
		}	else{result = DEAD_TASK; goto exit;}		//Устанавливать на выполнение висячие задачи нельзя!											
	  }	  
  } 
  
exit:  
  }//if (nointerrupted) {_enable_interrupts();}			// Разрешаем прерывания    
  return result; // return c кодом ошибки - нет свободных таймеров, таймер перезаписан или добавлен как новый	
}
#endif
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
               //if(TTask[index].TaskStatus != DEAD) {TTask[index].TaskStatus = RDY;} //Зависшие задачи никогда не станут выполняться
			   TTask[index].TaskStatus = RDY; 
               #ifdef USE_TTASKS_LOGGING
                TTask[index].run_me_cnt++;      //пора запустить задачу ++ раз
               #endif 
              }      
		}
	} 
}


//===============================================================================================
//Диспетчер задач ОС. Выбирает из очереди задачи и отправляет на выполнение
//===============================================================================================
inline void TaskManager(void) //в диспетчере задач выполняются задачи из очереди TTask, которые уже "выщелкали"
{
#ifdef DEBUG 
char 		tmp_str[10];
#endif
uint8_t		index = 0;
bit 		task_exist = 1;// существует ли задача всё ещё
TPTR 		CurrentTask;     
          
  for(index=0;index!=timers_cnt_tail;++index)   // Прочесываем очередь задач
	{	
      if ((TTask[index].TaskStatus == RDY)) // Если задача отмечена к выполнению (пропускаем пустые и те, время которых еще не подошло)
		{
#ifdef DEBUG        
          _LED1_ON;   //Для отслеживания загрузки системы  
#endif          
          CurrentTask = TTask[index].GoToTask;  // запомним задачу т.к. во время выполнения может измениться индекс

           if(TTask[index].TaskPeriod == 0) //если период 0 - удаляем задачу из списка
           {
                ClearTimerTask(CurrentTask);  task_exist = 0;// задачи больше не существует
           } 
           else 
           {
                TTask[index].TaskDelay = TTask[index].TaskPeriod; //перезапись задержки
                TTask[index].TaskStatus = IN_PROC;  //Задача в процессе выполнения     
#ifdef USE_TTASKS_LOGGING                                    //запись св-ств задачи для лога
                TTask[index].sys_tick_time = v_u32_SYS_TICK; //время начала выполнения                                  
#endif       
           }     
             //Дальше идём на выполнение задачи
 //----------------------------------------------------------------------------------------------------      
 #ifdef USE_TTASKS_LOGGING 
            v_u8_SYS_TICK_TMP1 = (uint8_t)v_u32_SYS_TICK; //засекаем время выполнения задачи     
 #endif 
            _enable_interrupts();						// Разрешаем прерывания
           (CurrentTask)();			        // ПЕРЕХОД К ЗАДАЧЕ!       
           
#ifdef USE_CORPSE_SERVISE            
InfiniteLoopFlag = 0; //Если задача зависнет - то в прерывании об этом узнают и прибьют по таймауту!
#endif
 //----------------------------------------------------------------------------------------------------
		
            if(task_exist)//если задача ранее не удалилась  
			{  
			   if(TTask[index].TaskStatus != DEAD) //Если задача не была отмечена в прерывании как зависшая
			   {
				TTask[index].TaskStatus = DONE; //меняем статус - благополучно выполнилась! 
			   }
#ifdef USE_TTASKS_LOGGING                     
				v_u8_SYS_TICK_TMP1 = (uint8_t)v_u32_SYS_TICK - v_u8_SYS_TICK_TMP1;   
                TTask[index].exec_time = v_u8_SYS_TICK_TMP1;//запишем время её выполнения   
                TTask[index].run_me_cnt--;      //задача выполнилась -- раз  
#endif
                
#ifdef DEBUG 				
				 if(v_u8_SYS_TICK_TMP1)
				 {
					itoa(v_u8_SYS_TICK_TMP1,tmp_str);
					Put_In_Log(tmp_str);Put_In_Log("%\r");
				 } 
                 if(TTask[index].run_me_cnt > 0) //Значит было пропущено выполнение задачи N-times!
				 {
					itoa(TTask[index].run_me_cnt,tmp_str);
					Put_In_Log(tmp_str);Put_In_Log("-N\r");
                    TTask[index].run_me_cnt = 0;
				 } 
#endif  			 
             }  
             //всё стопорится на єтой строке если не return!!!
            return;                             // выход до следующего цикла
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
//bit 		nointerrupted = 0;

//if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts(); nointerrupted = 1;}
ATOMIC_BLOCK_RESTORESTATE{
    for(index=0; index<timers_cnt_tail; ++index)
    {
      if(TTask[index].GoToTask == TS)
      {                                         
           if(index != (timers_cnt_tail - 1))         // переносим последнюю задачу
         {                                            // на место удаляемой
            TTask[index] = TTask[timers_cnt_tail - 1];      
            //зануление последней задачи (ДЛЯ ЭКОНОМИИ ВРЕМЕНИ - МОЖНО НЕ ЗАНУЛЯТЬ!)
            TTask[timers_cnt_tail - 1].GoToTask = Idle;
            TTask[timers_cnt_tail - 1].TaskDelay = 0; // Обнуляем время      
            TTask[timers_cnt_tail - 1].TaskPeriod = 0; // Обнуляем время     
            TTask[timers_cnt_tail - 1].TaskStatus = DONE; // Обнуляем status   
            #ifdef USE_TTASKS_LOGGING   
            TTask[timers_cnt_tail - 1].run_me_cnt = 0; // Обнуляем кол-во запусков
            #endif 
         }
           else//Если задача последняя в очереди
         {
            TTask[index].GoToTask = Idle;
            TTask[index].TaskDelay = 0; // Обнуляем время      
            TTask[index].TaskPeriod = 0; // Обнуляем время  
            TTask[index].TaskStatus = DONE; // Обнуляем status  
            #ifdef USE_TTASKS_LOGGING   
            TTask[index].run_me_cnt = 0; // Обнуляем кол-во запусков
            #endif 
         }            
         
        --timers_cnt_tail;  //уменьшаем кол-во задач     
        }//if (nointerrupted){ _enable_interrupts();}
        return;
      }
    }
}
//===============================================================================================


//===============================================================================================
#ifdef USE_SORTING_TTASK_QUEUE 
  void KERNEL_Sort_TaskQueue (void) //сортировкa задач по периоду выполнения (наиболее частые - ближе к началу очереди!)  
 {     
  TASK_STRUCT 	tmp;        
  int8_t 		l, r, k, index;   
  //bit			nointerrupted = 0;

 //if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания	
 ATOMIC_BLOCK_RESTORESTATE{
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
  }//if (nointerrupted){_enable_interrupts();}	// Разрешаем прерывания     
 } 
 #endif 
 //===============================================================================================

 

 //=============================================================================================== 
 #ifdef USE_CORPSE_SERVISE
 inline void CorpseService(void) //Обработка зависших задач
{
 static TPTR 		DeadTask_prev, DeadTask_curr;
 static uint16_t 	Timeout_delay = 0;
 static uint8_t 	coins = 0; //совпадения
 uint8_t			index = 0;   
 uint8_t			DeadTaskLocalTimeout = 0;   
 static bit 		suspect_flag = 0;  
 //bit				nointerrupted = 0;
  
  if(InfiniteLoopFlag == 0) //диспетчер сбросил флаг, значит задача завершилась
  {   
	goto EXIT;
  }
  else  //флаг не сброшен - выполняется какая-то задача! возможно уже долго или вообще зависла
  {
	//if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts();nointerrupted = 1;}	// Проверка запрета прерывания	
    ATOMIC_BLOCK_RESTORESTATE
    {    
	for(index=0; index<timers_cnt_tail; ++index)	//поиск мертвеца (пока ещё просто возможного!)
			 {   
				if((TTask[index].TaskStatus == IN_PROC) && (TTask[index].deadtime !=0xFF)) //фильтр задач на игнорирование
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
						if(DeadTask_curr == DeadTask_prev)	//подозрения подтвердились
							{
							    coins++;
								if(TTask[index].deadtime==0){DeadTaskLocalTimeout = DeadTaskDefaultTimeout;}
								else{DeadTaskLocalTimeout = TTask[index].deadtime;}
								
								if((v_u32_SYS_TICK - Timeout_delay >= DeadTaskLocalTimeout)&&(coins>=4))
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
   }//if (nointerrupted){_enable_interrupts();}	// Разрешаем прерывания     
} 
#endif   
//===============================================================================================























unsigned int db_swstk, db_hwstk;
unsigned char db_regs[32], db_sreg;

 inline extern void ContextSave(void)
 {
        #asm 
        ;save the two stack pointers for viewing in the debugger  
        sts _db_swstk, r28 
        sts _db_swstk+1, r29
        in r28, 0x3d   ;stack ptr
        in r29, 0x3e
        sts _db_hwstk, r28
        sts _db_hwstk+1, r29
        in  r28, 0x3f ;sreg
        sts _db_sreg, r28
        lds r28, _db_swstk
        lds r29, _db_swstk+1 
        ;save registers for viewing 
        sts _db_regs,r0 
        sts _db_regs+1,r1 
        sts _db_regs+2,r2 
        sts _db_regs+3,r3 
        sts _db_regs+4,r4 
        sts _db_regs+5,r5 
        sts _db_regs+6,r6 
        sts _db_regs+7,r7 
        sts _db_regs+8,r8 
        sts _db_regs+9,r9 
        sts _db_regs+10,r10 
        sts _db_regs+11,r11 
        sts _db_regs+12,r12 
        sts _db_regs+13,r13 
        sts _db_regs+14,r14 
        sts _db_regs+15,r15 
        sts _db_regs+16,r16 
        sts _db_regs+17,r17 
        sts _db_regs+18,r18 
        sts _db_regs+19,r19 
        sts _db_regs+20,r20 
        sts _db_regs+21,r21 
        sts _db_regs+22,r22 
        sts _db_regs+23,r23 
        sts _db_regs+24,r24 
        sts _db_regs+25,r25 
        sts _db_regs+26,r26 
        sts _db_regs+27,r27 
        sts _db_regs+28,r28 
        sts _db_regs+29,r29 
        sts _db_regs+30,r30 
        sts _db_regs+31,r31 
        #endasm  
} //end ContextSave
 
inline extern void ContextRestore(void)
{ 
//void loaddatareg(void)
        #asm    
        lds r0, _db_regs 
        lds r1, _db_regs+1 
        lds r2, _db_regs+2 
        lds r3, _db_regs+3 
        lds r4, _db_regs+4 
        lds r5, _db_regs+5 
        lds r6, _db_regs+6 
        lds r7, _db_regs+7 
        lds r8, _db_regs+8 
        lds r9, _db_regs+9 
        lds r10, _db_regs+10 
        lds r11, _db_regs+11 
        lds r12, _db_regs+12 
        lds r13, _db_regs+13 
        lds r14, _db_regs+14 
        lds r15, _db_regs+15 
        lds r16, _db_regs+16 
        lds r17, _db_regs+17 
        lds r18, _db_regs+18 
        lds r19, _db_regs+19 
        lds r20, _db_regs+20 
        lds r21, _db_regs+21 
        #endasm  
} //end loaddatareg    

 //TODO look at http://we.easyelectronics.ru/Soft/minimalistichnaya-ochered-zadach-na-c.html
 //TODO look at http://we.easyelectronics.ru/Soft/dispetcher-snova-dispetcher.html
 //http://habrahabr.ru/post/58366/ event-driven system!!!
 //http://www.femtoos.org/features.html!!!!
 //http://www.cs.ucr.edu/~vahid/rios/ различные варианты диспетчера
 
 
 //===============================================================================================
 //===============================================================================================
 //===============================================================================================
 //Очистка очереди от дубликатов задач с разным временем
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
//===============================================================================================

//Дебажные выводы
 //===============================================================================================  
#ifdef DEBUG 
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

#endif

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


//В задачах юзать автоматы! Возможно сделась задачу с параметром и возвращаемым значением!

/*Возможно стоит вести отсчёт пропущенных (RunMe) запусков так же void scheduler_update(void) interrupt 
{
    foreach (task in all_task_list)
    {
        task.PeriodCur--;
        if (task.PeriodCur == 0)
        {
            task.PeriodCur = task.Period;
            task.RunMe++;
        }
    }
}

void dispatch_tasks(void)
{
    foreach (task in all_task_list)
    {
        if (task.RunMe > 0)
        {
            task.pTask();
            task.RunMe--;
        }
    }
}


*/