#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"


/*
Насколько хорошо это будет работать, если таймер установить на 50-100мкс при 16МГц?

DI HALT:
9 Апрель 2012 в 21:08
Плохо будет работать. Там довольно многое можно оптимизировать,
среднее выполнение блока таймерной службы сейчас около 600 тактов.
Ну или около того. Столько же в среднем занимает перебор очереди,
установка таймера порядка 700 тактов, установка задачи около 400 тактов.
Реально тайминг снизить до 500мкс ,но делать очень коротки задачи, иначе таймер лажать будет.
*/


/*
  UPDATE - Избавился от очереди задач TaskQueue, вместо этого в диспетчере задач
  выполняются задачи из очереди MainTimer, которые уже "выщелкали",
  соответственно количество кода очень уменьшилось.
  MEMORY -58 WORDS
*/



// Очереди задач, таймеров.
// Тип данных - указатель на функцию
//volatile static TPTR	TaskQueue[TaskQueueSize+1];			// очередь указателей
//update
#warning оптимизировать передачей указателя или ссылки а структуру
volatile static struct
						{     
                            uint16_t Time;					// Выдержка в мс
						    TPTR GoToTask; 						// Указатель перехода		
                            //TODO добавить параметр и отладить				    
						}
						MainTimer[MainTimerQueueSize+1];	// Очередь таймеров


// RTOS Подготовка. Очистка очередей
  void InitRTOS(void)
{
uint8_t	index;

      for(index=0;index!=MainTimerQueueSize+1;index++) // Обнуляем все таймеры.
    {
	    MainTimer[index].GoToTask = Idle;
	    MainTimer[index].Time = 0;
	 }
}


//Пустая процедура - простой ядра.
  void  Idle(void)
{
  #warning наполнить полезным функционалом
}

 //UPDATE
 void SetTask(TPTR TS){  // Поставить задачу в очередь для немедленного выполнения
 SetTimerTask(TS,0);
}


//Функция установки задачи по таймеру. Передаваемые параметры - указатель на функцию,
// Время выдержки в тиках системного таймера. Возвращет код ошибки.

void SetTimerTask(TPTR TS, unsigned int NewTime)    //1 task ~12words
{
uint8_t		index=0;
uint8_t		nointerrupted = 0;

if (STATUS_REG & (1<<Interrupt_Flag)) 			// Проверка запрета прерывания, аналогично функции выше
	{
	_disable_interrupts()
	nointerrupted = 1;
	}
//====================================================================
// My UPDATE - not optimized
  for(index=0;index!=MainTimerQueueSize+1;++index)	//Прочесываем очередь таймеров
	{
	if(MainTimer[index].GoToTask == TS)				// Если уже есть запись с таким адресом
		{
		MainTimer[index].Time = NewTime;			// Перезаписываем ей выдержку
		if (nointerrupted) 	_enable_interrupts()	// Разрешаем прерывания если не были запрещены.
		return;										// Выходим. Раньше был код успешной операции. Пока убрал
		}
	}
  for(index=0;index!=MainTimerQueueSize+1;++index)	// Если не находим похожий таймер, то ищем любой пустой
	{
	if (MainTimer[index].GoToTask == Idle)
		{
		MainTimer[index].GoToTask = TS;			// Заполняем поле перехода задачи
		MainTimer[index].Time = NewTime;		// И поле выдержки времени
		if (nointerrupted) 	_enable_interrupts()	// Разрешаем прерывания
		return;									// Выход.
		}

	}
//====================================================================
/*
  for(index=0;index!=MainTimerQueueSize+1;++index)	//Прочесываем очередь таймеров
	{
	if(MainTimer[index].GoToTask == TS)				// Если уже есть запись с таким адресом
		{
		MainTimer[index].Time = NewTime;			// Перезаписываем ей выдержку
		if (nointerrupted) 	_enable_interrupts()		// Разрешаем прерывания если не были запрещены.
		return;										// Выходим. Раньше был код успешной операции. Пока убрал
		}
	}
  for(index=0;index!=MainTimerQueueSize+1;++index)	// Если не находим похожий таймер, то ищем любой пустой
	{
	if (MainTimer[index].GoToTask == Idle)
		{
		MainTimer[index].GoToTask = TS;			// Заполняем поле перехода задачи
		MainTimer[index].Time = NewTime;		// И поле выдержки времени
		if (nointerrupted) 	_enable_interrupts()	// Разрешаем прерывания
		return;									// Выход.
		}

	}	*/								// тут можно сделать return c кодом ошибки - нет свободных таймеров
}

/*=================================================================================
Диспетчер задач ОС. Выбирает из очереди задачи и отправляет на выполнение.
*/

inline void TaskManager(void)
{
uint8_t		index=0;
TPTR task;                 //TODO сделать глобальными регистровыми!

  for(index=0;index!=MainTimerQueueSize+1;++index) {  // Прочесываем очередь в поисках нужной задачи
		if ((MainTimer[index].GoToTask != Idle)&&(MainTimer[index].Time==0)) // пропускаем пустые задачи и те, время которых еще не подошло
		{
            task=MainTimer[index].GoToTask;             // запомним задачу
		    MainTimer[index].GoToTask = Idle;           // ставим затычку
            _enable_interrupts()							// Разрешаем прерывания
            (task)();								    // Переходим к задаче
            return;                                     // выход до следующего цикла
		}
	}
    _enable_interrupts()							// Разрешаем прерывания
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

for(index=0;index!=MainTimerQueueSize+1;index++)		// Прочесываем очередь таймеров
	{
//==========================================================================
//UPDATE
         if((MainTimer[index].GoToTask != Idle) && 		    // Если не пустышка и
           (MainTimer[index].Time > 0)) {					// таймер не выщелкал, то
            MainTimer[index].Time--;						// щелкаем еще раз.
		};
	}
}

//добавление от sniuk 7.1.14
void ClearTimerTask(TPTR TS)  //обнуление таймера
{
uint8_t	 index=0;
uint8_t nointerrupted = 0;
if (STATUS_REG & (1<<Interrupt_Flag))
{
_disable_interrupts();
nointerrupted = 1;
}
    for(index=0; index!=MainTimerQueueSize+1; ++index)
    {
        if(MainTimer[index].GoToTask == TS)
        {
            MainTimer[index].GoToTask = Idle;
            MainTimer[index].Time = 0; // Обнуляем время
            if (nointerrupted) _enable_interrupts();
            return;
        }
    }
}


  //TODO look at http://we.easyelectronics.ru/Soft/minimalistichnaya-ochered-zadach-na-c.html