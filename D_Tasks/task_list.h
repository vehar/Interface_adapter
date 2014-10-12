#include "adapter.h"


// Прототипы задач ============================================================
void Task_Start (void);
void Task_LedOff (void);
void Task_LedOn (void);
void Task_ADC_test (void); //Upd-6
void Task5 (void);
void Task6 (void);
void Task7 (void);
void Task8 (void);
void Task9 (void);

void Task_LcdGreetImage (void);
void Task_LcdLines (void);


void Task_pars_cmd (void);      //Запуск парсера
void Task_LogOut (void);        // Выброс логов
void Task_BuffOut (void);       // Вывод содержимого кольцевого буффера
void Task_Flush_WorkLog(void);  //очистка лог буффера
void Task_SPI_ClrBuf (void);    //очистка rx/tx буфферов SPI

//======================I2C===================================
// Прототипы задач ============================================================
void EEP_Writed(void);
void EEP_StartWrite(void);
void IIC_Send_Addr_ToSlave(void);
void IIC_SendeD_Addr_ToSlave(void);
void SlaveControl(void);
