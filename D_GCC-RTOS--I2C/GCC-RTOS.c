#include <HAL.h>
#include <EERTOS.h>


// Прототипы задач ============================================================
void Writed2EEP(void);
void StartWrite2EPP(void);
void SendAddrToSlave(void);
void SendedAddrToSlave(void);
void SlaveControl(void);

// void LogOut(void);
// Global Variables

u08 UART_RX;


//RTOS Interrupt
ISR(RTOS_ISR)
{
TimerService();						// Прерывание ядра диспетчера
}
//..........................................................................

ISR(USART_RXC_vect)
{
UART_RX = UDR;						// Сгребаем принятый байт в буфер
SetTask(StartWrite2EPP);			// Запускаем процесс записи в ЕЕПРОМ.
}



//============================================================================
//Область задач
//============================================================================

// Записываем в ЕЕПРОМ байт. 
void StartWrite2EPP(void)
{
if (!i2c_eep_WriteByte(0xA0,0x00FF,UART_RX,&Writed2EEP))	// Если байт незаписался
	{
	SetTimerTask(StartWrite2EPP,50);						// Повторить попытку через 50мс
	}
}

// Точка выхода из автомата по записи в ЕЕПРОМ
void Writed2EEP(void)
{
i2c_Do &= i2c_Free;											// Освобождаем шину

if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))						// Если запись не удалась
	{
	SetTimerTask(StartWrite2EPP,20);						// повторяем попытку
	}
else
	{
	SetTask(SendAddrToSlave);								// Если все ок, то идем на следующий
	}														// Пункт задания - передача данных слейву 2
}



// Обращение к SLAVE контроллеру
void SendAddrToSlave(void)
{
if (i2c_Do & i2c_Busy)						// Если передатчик занят
		{
		SetTimerTask(SendAddrToSlave,100);	// То повторить через 100мс
		}

i2c_index = 0;								// Сброс индекса
i2c_ByteCount = 2;							// Шлем два байта

i2c_SlaveAddress = 0xB0;					// Адрес контроллера 0xB0

i2c_Buffer[0] = 0x00;						// Те самые два байта, что мы шлем подчиненному
i2c_Buffer[1] = 0xFF;

i2c_Do = i2c_sawp;							// Режим = простая запись, адрес+два байта данных

MasterOutFunc = &SendedAddrToSlave;			// Точка выхода из автомата если все хорошо
ErrorOutFunc = &SendedAddrToSlave;			// И если все плохо. 

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;		// Поехали!

i2c_Do |= i2c_Busy;												// Шина занята!
}


// Выход из автомата IIC
void SendedAddrToSlave(void)
{
i2c_Do &= i2c_Free;							// Освобождаем шину


if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))		// Если адресат нас не услышал или был сбой на линии
	{										
	SetTimerTask(SendAddrToSlave,20);		// Повторить попытку
	}
}


// Если словили свой адрес и приняли данные
void SlaveControl(void)
{
i2c_Do &= i2c_Free;				// Освобождаем шину
UDR = i2c_InBuff[0];			// Выгружаем принятый байт
}

/*
void LogOut(void)				// Выброс логов
{
u08 i;

WorkLog[WorkIndex]= 0xFF;
WorkIndex++;

for(i=0;i!=WorkIndex+1;i++)
	{
	UDR = WorkLog[i];
	_delay_ms(30);
	}
}
*/

//==============================================================================
int main(void)
{
InitAll();						// Инициализируем периферию
Init_i2c();						// Запускаем и конфигурируем i2c
Init_Slave_i2c(&SlaveControl);	// Настраиваем событие выхода при сработке как Slave

/*
WorkIndex=0;					// Лог с начала
WorkLog[WorkIndex]=1;			// Записываем метку старта
WorkIndex++;					
*/

InitRTOS();			// Инициализируем ядро
RunRTOS();			// Старт ядра. 

_delay_ms(1);		// Небольшая выдержка, чтобы второй контроллер успел встать на адресацию


while(1) 		// Главный цикл диспетчера
{
wdt_reset();	// Сброс собачьего таймера
TaskManager();	// Вызов диспетчера
}

return 0;
}



