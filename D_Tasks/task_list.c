//Определение всех задач для RTOS

#include "task_list.h"
//============================================================================
//Область задач
//============================================================================


DECLARE_TASK(Task_LoadTest)
{
 // LED_PORT  &= ~(1<<LED2);
 //delay_ms(10);      
 // LED_PORT |= (1<<LED2);
 //delay_ms(10);
          
  LcdClear();    
  sprintf (lcd_buf, "T=%i",v_u32_SYS_TICK);  
 LcdString(1,2);      
 LcdUpdate();    
 
SetTimerTask(Task_LoadTest,2000);; //запуск тестового таска для проверки загрузки цп
}

DECLARE_TASK(Task_Start)
{
SetTimerTask(Task_LedOff,100); //запуск мигалки-антизависалки
}

/*
void Task_Start (void)
{
SetTimerTask(Task_LedOff,100); //запуск мигалки-антизависалки
}
  */
DECLARE_TASK (Task_LedOff) 
{
SetTimerTask(Task_LedOn,900);
LED_PORT  &= ~(1<<LED2);
}

DECLARE_TASK (Task_LedOn)
{
SetTimerTask(Task_LedOff,100);
LED_PORT  |= (1<<LED2);
}



DECLARE_TASK (Task_ADC_test) //Upd-6     //для проверки освещённоси помещения
 {
 #warning no Lcd_out// sprintf (lcd_buf, "DummyADC=%d ",volt);      // вывод на экран результата
 LcdString(1,1);   LcdUpdate();
 SetTimerTask(Task_ADC_test,5000);
 }
void Task_LcdGreetImage (void) //Greeting image on start    //Upd-4
{
//SetTask(LcdClear);
//SetTask(Task_LcdLines);
 //LcdImage(rad1Image);  SetTimerTask(LcdClear,3000);
      
 //sprintf (lcd_buf, "  Interface   ");    
 strncpy (lcd_buf, "  Interface   ", 15);
 LcdStringBig(1,1);  
  strncpy(lcd_buf, " Monitor v2.1 ", 15);    
 LcdString(1,3);    
 strncpy (lcd_buf, " Хмелевськой  ", 15);    
 LcdString(1,4);  
 strncpy (lcd_buf, "  Владислав   ", 15);    
 LcdString(1,5);   
 strncpy (lcd_buf, "  АЕ - 104    ", 15);    
 LcdString(1,6);  
LcdUpdate();
//SetTimerTask(LcdClear,7000);

// sprintf (lcd_buf, "Wait comand...");      //не работает
// LcdStringBig(1,3);  
// SetTimerTask(LcdUpdate,8000);
}

void Task_LcdLines (void)      //Upd-4       //сильно грузит!
{
    	for (i=0; i<84; i++)
        {
		LcdLine ( 0, 47, i, 0, 1);
        LcdLine ( 84, 47, 84-i, 0, 1);
		LcdUpdate();
		}
}

void Task_AdcOnLcd (void)
{
//SetTask(LcdClear);
 /*sprintf (lcd_buf, "vref=%d ",vref);      // вывод на экран результата
 LcdString(1,1);      
  sprintf (lcd_buf, "d=%d ",d);      // вывод на экран результата
 LcdString(1,2);
  sprintf (lcd_buf, "delta=%d ",delta);      // вывод на экран результата
 LcdString(1,3);
  sprintf (lcd_buf, "volt=%d ",volt);      // вывод на экран результата
 LcdString(1,4);  */
 LcdClear();    
 ADC_use();
#warning disabled sprintf (lcd_buf, "Напряжение на");  
 LcdString(1,2);   
#warning disabled sprintf (lcd_buf, "  канале 0");  
 LcdString(1,3); 
#warning disabled sprintf (lcd_buf, "= %d мВ",adc_result);  
 LcdString(1,4);
 
 LcdUpdate();
}

//#error Проверить!
void Task_pars_cmd (void)
{
char scan_interval = 100;
  if (USART_Get_rxCount(SYSTEM_USART) > 0) //если в приёмном буфере что-то есть
       {
        symbol = USART_Get_Char(SYSTEM_USART);
        PARS_Parser(symbol);
        //SetTask(Task_pars_cmd);  //Проверить!    
        scan_interval = 10;
       } 
 else{scan_interval = 100;};
SetTimerTask(Task_pars_cmd, scan_interval); //25   //Проверить!
}


void Task_LogOut (void)
{
SetTimerTask(Task_LogOut,50);
if(LogIndex){LogOut();} //если что-то есть в лог буфере - вывести
}


void Task_Flush_WorkLog(void){ //очистка лог буффера
uint16_t i = 0;
while(i<512){WorkLog[i] = 0; i++;};
PORTD.7^=1;
}

/*
void Task_Uart_ByfSend(void){ //очистка лог буффера

}*/

void Task_SPI_ClrBuf (void){ //очистка rx/tx буфферов SPI
uint8_t i;
for(i=0;i<64;i++)
 {
Spi0_RX_buf[i] = 0;
Spi0_TX_buf[i] = 0;
  //if(i<=SIZE_SPI_BUF_TX){Spi0_TX_buf[i] = 0;}
 }
}

 void Task_BuffOut  (void)
 {
  SetTimerTask(Task_BuffOut,50);   
  RingBuff_TX();
 }

//=================================================
//////////////////////////I2C//////////////////////

//============================================================================
//Область задач
//============================================================================

// Записываем в ЕЕПРОМ байт.
void EEP_StartWrite(void)
{
if (!i2c_eep_WriteByte(0xA0,0x00FF,/*(char)Usart0_RX_buf[15]*/ 9,&EEP_Writed))    // Если байт незаписался
    {
    SetTimerTask(EEP_StartWrite,50);                        // Повторить попытку через 50мс
    }
}

// Точка выхода из автомата по записи в ЕЕПРОМ
void EEP_Writed(void)
{
i2c_Do &= i2c_Free;                                            // Освобождаем шину

if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))                        // Если запись не удалась
    {
    SetTimerTask(EEP_StartWrite,20);                        // повторяем попытку
    }
else
    {
    SetTask(IIC_Send_Addr_ToSlave);        		// Если все ок, то идем на следующий
	}											// Пункт задания - передача данных слейву 2
}

// Обращение к SLAVE контроллеру
void IIC_Send_Addr_ToSlave(void)
{
if (i2c_Do & i2c_Busy)						// Если передатчик занят
		{
		SetTimerTask(IIC_Send_Addr_ToSlave,100);	// То повторить через 100мс
		}

i2c_index = 0;								// Сброс индекса
i2c_ByteCount = 2;							// Шлем два байта

i2c_SlaveAddress = 0xB0;					// Адрес контроллера 0xB0

i2c_Buffer[0] = 0x00;						// Те самые два байта, что мы шлем подчиненному
i2c_Buffer[1] = 0xFF;

i2c_Do = i2c_sawp;							// Режим = простая запись, адрес+два байта данных

MasterOutFunc = &IIC_SendeD_Addr_ToSlave;			// Точка выхода из автомата если все хорошо
ErrorOutFunc = &IIC_SendeD_Addr_ToSlave;			// И если все плохо.

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;		// Поехали!
i2c_Do |= i2c_Busy;												// Шина занята!
}


// Выход из автомата IIC
void IIC_SendeD_Addr_ToSlave(void)
{
i2c_Do &= i2c_Free;							// Освобождаем шину

if(i2c_Do & (i2c_ERR_NA|i2c_ERR_BF))		// Если адресат нас не услышал или был сбой на линии
	{
	SetTimerTask(IIC_Send_Addr_ToSlave,20);		// Повторить попытку
	}
}


// Если словили свой адрес и приняли данные
void SlaveControl(void)
{
i2c_Do &= i2c_Free;				// Освобождаем шину
UDR0 = i2c_InBuff[0];			// Выгружаем принятый байт
}

//==============================================================================
