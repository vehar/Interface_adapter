#ifndef EERTOS_H
#define EERTOS_H

#include "RTOS/HAL.h"
#include "RTOS/EERTOSHAL.h"

extern void InitRTOS(void);
extern void Idle(void);

typedef uint8_t T_ARG;
typedef void (*T_PTR)(T_ARG);//заготовка для задач с параметром

typedef void (*TPTR)(void);

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
