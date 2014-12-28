#ifndef EERTOS_H
#define EERTOS_H

#include "RTOS/HAL.h"
#include "RTOS/EERTOSHAL.h"

extern void InitRTOS(void);
extern void Idle(void);

typedef uint8_t T_ARG;
typedef void (*T_PTR)(T_ARG);//заготовка для задач с параметром

typedef void (*TPTR)(void);

#ifdef DEBUG
 extern  volatile uint32_t v_u32_SYS_TICK;
 extern volatile uint16_t v_u16_TIM_1_OVR_FLAG;
 extern void Timer_3_start(void);
 extern void Timer_3_stop(void);
 extern void Put_In_Log (unsigned char * data);
 extern uint16_t Timer_3_get_val (void);
#endif


extern void SetTask(TPTR TS);
extern void SetTimerTask(TPTR TS, unsigned int NewTime);

inline extern void TaskManager(void);
inline extern void TimerService(void);

extern void ClearTimerTask(TPTR TS);

//RTOS Errors Пока не используются.
#define TaskSetOk			 'A'
#define TaskQueueOverflow	 'B'
#define TimerUpdated		 'C'
#define TimerSetOk			 'D'
#define TimerOverflow		 'E'

#endif
