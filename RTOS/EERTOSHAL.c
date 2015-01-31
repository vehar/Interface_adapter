#include "RTOS/EERTOSHAL.h"

//RTOS Запуск системного таймера
void RTOS_timer_init (void)
{     
	TCNT2 = 0;                                // Установить начальное значение счётчиков
	OCR2  = LO(RtosTimerDivider);         		// Установить значение в регистр сравнения
	TIMSK = (0<<TOIE2)|(1<<OCIE2);
}

void RunRTOS (void)
{
	#asm("cli");     
	TCCR2 = (1<<WGM21)|(1<<CS22)|(0<<CS20)|(0<<CS21);    // Freq = CK/256 - Установить режим и предделитель
	#asm("sei");// Разрешаем прерывание RTOS - запуск ОС // Автосброс после достижения регистра сравнения
}                             


  #warning поэкспериментировать с переменным RtosTimerDivider в StopRTOS и в зависимости от задач(возможно их приоритета)
//RTOS увеличение предделителя системного таймера
void StopRTOS (void)//Фактически снижение частоты системного таймера
{
	#asm("cli");
	TCCR2 = (0<<CS21)|(1<<CS22)|(1<<CS20); // Freq = CK/1024
	#asm("sei");
}


//RTOS Остановка системного таймера
void FullStopRTOS (void)
{
	#asm("cli");
	TCCR2 = 0;                        // Сбросить режим и предделитель
	TIMSK = (0<<TOIE2)|(0<<OCIE2);	 // запрещаем прерывание RTOS - остановка ОС
	#asm("sei");
}


void DeadTimerInit (void)
{
	TCCR0 = (1<<WGM01)|(1<<CS02)|(0<<CS00)|(0<<CS01);
	TCNT0=0x00;
	OCR0=LO(DeadTimerDivider);
	TIMSK = (0<<TOIE0)|(1<<OCIE0);
}      