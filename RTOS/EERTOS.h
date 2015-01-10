#ifndef EERTOS_H
#define EERTOS_H

#include "RTOS/HAL.h"
#include "RTOS/EERTOSHAL.h"


#define QUEUE_SORTING_PERIOD 100 //ticks(!)  //можно увеличить, чтоб не грузить Idle-задачу
#define DEAD_TIMEOUT 7

extern void InitRTOS (void);
extern void InitRTOS(void);
extern void Idle(void);

extern void DeadTimerInit (void);
extern void DeadTimerRun (void);
extern void DeadTimerStop (void);

typedef uint8_t T_ARG;
typedef void (*T_PTR)(T_ARG);//заготовка для задач с параметром

typedef void (*TPTR)(void);
extern  volatile bit InfiniteLoopFlag;  
extern  volatile uint16_t DeadTaskTimeout;  

#ifdef DEBUG
 extern  volatile uint32_t v_u32_SYS_TICK;    
 extern  volatile uint8_t v_u8_SYS_TICK_TMP1;
 extern void Put_In_Log (unsigned char * data);
 
 extern volatile uint16_t v_u16_TIM_1_OVR_FLAG;
 extern void Timer_3_start(void);
 extern void Timer_3_stop(void);   
 
 extern void Put_In_Log (unsigned char * data); 
 extern void Task_LogOut(void);
 extern uint16_t Timer_3_get_val (void);    
 
 
//extern  TASK_STRUCT  TTask[MainTimerQueueSize+1];
extern volatile uint8_t timers_cnt_tail;
#endif


extern void SetTask(TPTR TS);
extern uint8_t SetTimerTask(TPTR TS, unsigned int NewTime, unsigned int NewPeriod);    //1 task ~12words

inline extern void TaskManager(void);
inline extern void TimerService(void);
inline extern void CorpseService(void);

extern void KERNEL_Sort_TaskQueue (void);

extern void ClearTimerTask(TPTR TS);

//RTOS Errors Пока не используются.
#define TaskSetOk			 'A'
#define TaskQueueOverflow	 'B'
#define TimerUpdated		 'C'
#define TimerSetOk			 'D'
#define TimerOverflow		 'E'

#endif
