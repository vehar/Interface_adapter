#ifndef EERTOS_H
#define EERTOS_H

#include "RTOS/EERTOSHAL.h"
  

//#define USE_MINIMAL_DESIGN  
  
#define USE_FULL_DESIGN

//#define USE_CUST_DESIGN

//Custom RTOS design

//#define USE_SORTING_TTASK_QUEUE

//#define USE_CORPSE_SERVISE 			//Delete dead_timeout_tasks from queue

//#define USE_TTASKS_LOGGING

//#define USE_UINT16_T_TASK_VS_UINT16_T_PARAMETER

//#define USE_VOID_TASK_VS_UINT16_T_PARAMETER

//#define USE_VOID_TASK_VS_VOID_PARAMETER

#ifdef USE_FULL_DESIGN
    
    #warning USED_FULL_DESIGN

	#define USE_SORTING_TTASK_QUEUE

	#define USE_CORPSE_SERVISE 			//Delete dead_timeout_tasks from queue

	#define USE_TTASKS_LOGGING
	
	//#define USE_UINT16_T_TASK_VS_UINT16_T_PARAMETER  
    
    #define USE_VOID_TASK_VS_VOID_PARAMETER 
   	
#elif USE_MINIMAL_DESIGN

    #warning USED_MINIMAL_DESIGN

	#define USE_VOID_TASK_VS_VOID_PARAMETER
	
#else	

	#ifndef USE_CUST_DESIGN 
    #error Конфигурация системы не указана!  
    #endif
        
#endif



#define TASK_QUEUE_SIZE	10
#define QUEUE_SORTING_PERIOD 100 //ticks(!)  //можно увеличить, чтоб не грузить Idle-задачу

#ifdef USE_VOID_TASK_VS_UINT16_T_PARAMETER
	typedef uint16_t T_ARG;
	typedef void (*TPTR)(T_ARG);//заготовка для задач с параметром
#endif

#ifdef USE_VOID_TASK_VS_VOID_PARAMETER
	typedef void (*TPTR)(void);
#endif

#ifdef USE_CORPSE_SERVISE
volatile static uint16_t DeadTaskDefaultTimeout = 10;
extern  volatile bit InfiniteLoopFlag;  
extern  volatile uint16_t DeadTaskDefaultTimeout;  
#endif

inline extern void Idle(void);

extern void InitRTOS(void); 
extern void SetTask(TPTR TS);
extern uint8_t SetTimerTask(TPTR TS, unsigned int NewTime, unsigned int NewPeriod); //1 task ~12words
#ifdef USE_CORPSE_SERVISE
extern uint8_t SetTaskDeadtime(TPTR TS, uint8_t DeadTime);
#endif
inline extern void TaskManager(void);
inline extern void TimerService(void);
extern void ClearTimerTask(TPTR TS);

#ifdef USE_CORPSE_SERVISE
inline extern void CorpseService(void); 
#endif 

#ifdef USE_SORTING_TTASK_QUEUE   
extern void KERNEL_Sort_TaskQueue (void);  
#endif    

inline extern void ContextSave(void);  
inline extern void ContextRestore(void); 

 extern  volatile uint32_t v_u32_SYS_TICK;     

#ifdef DEBUG 
 extern  volatile uint8_t v_u8_SYS_TICK_TMP1;
 extern void Put_In_Log (unsigned char * data);
 extern void Task_LogOut(void);       
#endif


//------------------------------------------------------------------
//---------------------ATOMIC_BLOCK--------------------------------- 
/*из такого atomic-блока нельзя принудительно выходить (при помощи goto/break/return), иначе прерывание не будет восстановлено.*/
extern bit			global_nointerrupted_flag;
extern uint8_t      global_interrupt_mask;
extern uint8_t      global_interrupt_cond;

inline static int _irqDis(void)
{
    _disable_interrupts();
    return 1;
}

inline static int _irqEn(void)
{
    _enable_interrupts();
    return 0;
}

#define ATOMIC_BLOCK_FORCEON \
    for(global_nointerrupted_flag = _irqDis();\
         global_nointerrupted_flag;\
         global_nointerrupted_flag = _irqEn())
//------------------------------------------------------------------
//------------------------------------------------------------------
/* Example
main()
{
    ATOMIC_BLOCK_FORCEON
    {
        do_something();
    }
}*/

inline static uint8_t _iDisGetPrimask(void)       //if (STATUS_REG & (1<<Interrupt_Flag)){_disable_interrupts(); nointerrupted = 1;}
{
    uint8_t result;
    result = STATUS_REG;
    _disable_interrupts();
    return result;
}

inline static uint8_t _iSetPrimask(uint8_t priMask)
{
    STATUS_REG = priMask;
    return 0;
}

#define ATOMIC_BLOCK_RESTORESTATE \
     for( global_interrupt_mask = _iDisGetPrimask(), global_nointerrupted_flag = 1;\
         global_nointerrupted_flag;\
         global_nointerrupted_flag = _iSetPrimask(global_interrupt_mask))
//------------------------------------------------------------------
//------------------------------------------------------------------


inline static uint8_t _irqCondDis(uint8_t flag)
{
    if (flag)_disable_interrupts();
    return 1;
}

inline static uint8_t _irqCondEn(uint8_t flag)
{
    if (flag)_enable_interrupts();
    return 0;
}

#define ATOMIC_BLOCK_FORCEON_COND(condition) \
    for(global_interrupt_cond = condition, global_nointerrupted_flag = _irqCondDis(global_interrupt_cond);\
        global_nointerrupted_flag;\
        global_nointerrupted_flag = _irqCondEn(global_interrupt_cond))
//------------------------------------------------------------------
//------------------------------------------------------------------        
 
inline static uint8_t _irqCondDisGetPrimask(uint8_t flag)
{
    uint8_t result;
    if (flag)
    {
    result = STATUS_REG;
    _disable_interrupts();
    }
    return result; 
}

inline static uint8_t _irqCondSetPrimask(uint8_t priMask)
{
    STATUS_REG = priMask;
    return 0;
}

#define ATOMIC_BLOCK_RESTORATE_COND(condition) \
    for(global_interrupt_cond = condition, global_interrupt_mask = _irqCondDisGetPrimask(global_interrupt_cond), global_nointerrupted_flag = 1;\
        global_nointerrupted_flag;\
        global_nointerrupted_flag = global_interrupt_cond ? _irqCondSetPrimask(global_interrupt_mask):0)

//---------------------ATOMIC_BLOCK--------------------------------- 
//------------------------------------------------------------------
//------------------------------------------------------------------         
         

//RTOS Errors Пока не используются.
#define TaskSetOk			 'A'
#define TaskQueueOverflow	 'B'
#define TimerUpdated		 'C'
#define TimerSetOk			 'D'
#define TimerOverflow		 'E'

#define _LED1_ON  LED_PORT|=(1<<LED1)
#define _LED1_OFF LED_PORT&=~(1<<LED1)
#endif
