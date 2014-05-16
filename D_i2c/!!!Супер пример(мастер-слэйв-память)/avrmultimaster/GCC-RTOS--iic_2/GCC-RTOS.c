#include <HAL.h>
#include <EERTOS.h>


// Прототипы задач ============================================================
void Writed2EEP(void);
void StartWrite2EPP(void);
void SendByteToSlave(void);
void SendedByteToSlave(void);
void ReadEEPROM(void);
void SlaveControl(void);
void EepromReaded(void);

//void LogOut(void);

// Global Variables

u08 ReadedByte;						// Байт считаный из ЕЕПРОМ


//RTOS Interrupt
ISR(RTOS_ISR)
{
TimerService();						// Прерывание ядра диспетчера
}
//..........................................................................





//============================================================================
//Область задач
//============================================================================
void SlaveControl(void)						// Точка выхода из автомата слейва
{
i2c_Do &= i2c_Free;							// Осовобождаем шину
SetTask(ReadEEPROM);						// Готовим запись в ЕЕПРОМ
}



void ReadEEPROM(void)									// Читаем из еепром
{
u16 Addr;

Addr = (i2c_InBuff[0]<<8)|(i2c_InBuff[1]);				// Адрес возьмем из буфера слейва

if (!i2c_eep_ReadByte(0xA0,Addr,1,&EepromReaded) )		// Читаем
	{
	SetTimerTask(ReadEEPROM,50);						// Если процесс не пошел (шина занята), то повтор через 50мс.
	}
}

void EepromReaded(void)					// Была попытка чтения
{
i2c_Do &= i2c_Free;						// Освобождаем шину

if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))	// Ошибки при четнии были?
	{
	SetTimerTask(ReadEEPROM,20);		// Тогда повтор
	}
else
	{
	ReadedByte = i2c_Buffer[0];			// Иначе считаный байт из буфера копируем в переменную
	SetTask(SendByteToSlave);			// И запускаем отсылку ее контроллеру 1
	}
}


void SendByteToSlave(void)			// Возвращаем контроллеру 1 его байт
{
if (i2c_Do & i2c_Busy)				// Если шина занята
		{
		SetTimerTask(SendByteToSlave,100);			// То повторяем попытку
		}

i2c_index = 0;							// Сброс индекса
i2c_ByteCount = 1;						// Шлем 1 байт

i2c_SlaveAddress = 0x32;				// Адрес контроллера 1 на шине

i2c_Buffer[0] = ReadedByte+1;			// Загружаем в буфер число, увеличив его на 1.
										// +1 чтобы понять, что число прошло через МК и было обработано 


i2c_Do = i2c_sawp;						// Режим - простая запись

MasterOutFunc = &SendedByteToSlave;		// Задаем точку выхода
ErrorOutFunc = &SendedByteToSlave;

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;		// Поехали!

i2c_Do |= i2c_Busy;												// Флаг занятости поставим
}


void SendedByteToSlave(void)				// Байт был послан
{
i2c_Do &= i2c_Free;							// Осовбождаем шину


if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))		// Если посылка не удалась
	{
	SetTimerTask(SendByteToSlave,20);		// Пробуем еще раз. 
	}
}											// Если все ок, то успокаиваемся. 


/*
void LogOut(void)
{
u08 i;

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
InitAll();							// Инициализируем периферию
Init_i2c();							// Настроили мастер режим
Init_Slave_i2c(&SlaveControl);		// Настроили слейв режим

/*
WorkLog[WorkIndex]=1;
WorkIndex++;
*/

InitRTOS();			// Инициализируем ядро
RunRTOS();			// Старт ядра. 



while(1) 		// Главный цикл диспетчера
{
wdt_reset();	// Сброс собачьего таймера
TaskManager();	// Вызов диспетчера
}

return 0;
}



