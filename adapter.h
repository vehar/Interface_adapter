#ifndef __MAIN_H
#define __MAIN_H

//*****PROJECT BUILD-CONTROL DEFINES*****//

//#define EEPROM_REINIT              //начальная инициализация еепром (выполняется 1 раз при компиляции)
//#define UART_BAUD-ERR-CONTROL_EN   //Проверка допустимости скорости, если ошибка велика, то автоматическая инициализация юарта на 4800



#include <mega128.h>

#include <i2c.h>
#include <1wire.h>
#include <ds1820.h>

#include <stdio.h> // Standard Input/Output functions

#include <delay.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <io.h>
#include <stdbool.h>

#include "D_LCD/n3310lcd_update.c"   //Upd-2   // D_LCD/ = Upd-8
#include "D_LCD/pictures.c"          //Upd-2   // D_LCD/ = Upd-8

#include "D_Globals/global_defines.h"     //Upd-8 in folder
#include "D_Globals/global_variables.h"   //Upd-8 in folder

#include "D_usart/usart.h"
#include "D_usart/usart.c"

#include "D_parser/parser.h"
#include "D_parser/parser.c"

#include "D_spi/spi.h"
#include "D_spi/spi.c"

#include "D_i2c/I2C.h"
#include "D_i2c/I2C.c"

#include "D_ADC/ADC.h"       //Upd-6
#include "D_ADC/ADC.c"       //Upd-6

#include "D_RTC/RTC.h"       //Upd-8 real-time-clock
#include "D_RTC/RTC.c"       //Upd-8


#include "D_IIC_ultimate/IIC_ultimate.h"
#include "D_IIC_ultimate/IIC_ultimate.c"
#include "D_i2c_AT24C_EEP/i2c_AT24C_EEP.h"
#include "D_i2c_AT24C_EEP/i2c_AT24C_EEP.c"


#include "D_Globals/watchdog.c"            //Upd-8 in folder
#include "D_Globals/interruptes.c"        //Upd-8 in folder

#include "D_Globals/global_funktions.c"   //Upd-8 in folder

#include "RTOS/HAL.h"
#include "RTOS/EERTOS.h"
#include "RTOS/EERTOSHAL.h"

//#include "task_list.h"
#include "D_Tasks/task_list.c"





#endif /* __MAIN_H */