//---------------------START UART VARS----------------//

//---------------------END UART VARS----------------//


#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <stdint.h>
#include "D_usart/usart.h"



  
   uint16_t g_tcf = 0; //_global_taskControl_flags
   enum g_tcf_values {  
                        ERR_UNDEF_FLAG, //ошибка - неопределЄнный флаг
                        S_SPI_BUF_CLR,
                        FLUSH_WORKLOG
   }; 
   
  volatile char U1_in_buf_flag = 0;
    
 
extern volatile uint8_t symbol = 0;
//extern volatile uint8_t Parser_req_state_cnt = 255;
 volatile  uint32_t v_u32_SYS_TICK; //may be static
 volatile  uint8_t v_u8_SYS_TICK_TMP1;

extern volatile uint32_t v_u32_TX_CNT = 0;
extern volatile uint32_t v_u32_RX_CNT = 0;

volatile uint16_t v_u16_TIM_1_OVR_FLAG = 0;

/*команды*/
__flash char Help[]   = "Help";
/*
  Help
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

__flash char Set[]   = "Set";

    __flash char Uart[]              = "Uart";
    //__flash char Uart1[]           = "U_1";
        __flash char Mode[]          = "Mode";//USART_NORMAL or USART_DOUBLED
        __flash char Speed[]         = "Speed";

    __flash char Spi[]   = "Spi";
        //__flash char Mode[]        = "Mode";
            //__flash char Master[]  = "Mst";
            //__flash char Slave[]   = "Slv";
        __flash char Prescaller[]    = "Presc";
        __flash char PhaPol[]        = "pp"; //phase and polarity mode

    __flash char I2c[]   = "I2c";
        //__flash char Master[]      = "Mst";
        //__flash char Slave[]       = "Slv";
        //__flash char Speed[]       = "Spd";
        __flash char Adr[]           = "Adr"; //set MY adress

    __flash char Rx[]     = "Rx"; //Recieve
        __flash char Auto[]          = "Auto"; //auto Recieve
        __flash char Man[]           = "Man"; //manual Recieve
/*
  Set "interface" 'number' "mode" 'value(optional)' "mode" 'value(optional)'
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

__flash char Get[]   = "Get";
    __flash char Settings[]   = "S";
    __flash char Data[]       = "D";
/*
  Get "sourse" "interface(optional)"
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

__flash char W[]   = "W";  //write

/*
  W "interface" 'value'
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

__flash char R[]   = "R";  //read
/*
  R "interface"
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

__flash char reset[]   = "rst";  //read
__flash char boot[]   = "boot";  //read
__flash char dbg[]      = "dbg";
/*
 —лужебные команды
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

/*ответы*/
__flash char error[]      = " E\r";
__flash char ok[]         = " ok\r";
__flash char yes[]        = " Y";
__flash char no[]         = " N";
__flash char largeValue[] = " Large_value\r";
__flash char wrongValue[] = " Wrong_value\r";

/*I2c/Twi block  */
__flash char start[]      = " S";
__flash char stop[]       = " P";
__flash char akk[]        = " Ak";
__flash char nakk[]       = " Nk";
__flash char address[]    = " A";
__flash char data[]       = " D";
//__flash char W[]        = "W";  //write
//__flash char R[]        = "R";  //read
__flash char timeslot[]   = " T"; //прин€ть/передать 1 тайм- слот
__flash char presense[]   = " Pr"; //прин€ть/передать 1 тайм- слот
/* */

__flash char help_mess_0[] = "\r>>> HELP <<<\r Commands:\r";
__flash char help_mess_1[] = "\r Set -Interface_name- -number- Mode -value(optional)- Speed -value(optional)-";
__flash char help_mess_2[] = "\r W   -Interface_name- -number- -data-";
__flash char help_mess_3[] = "\r Get -entity-";
__flash char help_mess_4[] = "\r  Interfaces available:\r   Uart(0,1)\r   Spi(0-Hard, 1-Soft)\r";

__flash char help_Uart_0[] = "\r>>> UART_HELP <<<\r Mode: 0- NORMAL; 1- DOUBLED speed";
__flash char help_Uart_1[] = "\r Speed: baud/100 (Ex: 576 is 57600)\r";

__flash char help_Spi_0[] = "\r>>> SPI_HELP <<<\r Mode: 0- SLAVE; 1- MASTER\n";
__flash char help_Spi_1[] = "\r Prescaller: pow of 2 (Ex: Presc 16)\r";


//’ранимые настройки

 #warning ќѕ“»ћ»«»–ќ¬ј“№!
 /* Global structure located in EEPROM */ 
 eeprom char null_ee;//дл€ услови€ начальной инициализации еепром
eeprom  struct eeprom_settings_structure {
 unsigned char MODE_of_Uart[COUNT_OF_UARTS]; 
 unsigned int baud_of_Uart[COUNT_OF_UARTS];
 //bool mode, bool phase, bool polarity, uint16_t prescaller
 unsigned char MODE_of_Spi[COUNT_OF_SPI]; // bits 0 - master/slave, 1 - phase, 2 - polarity
 unsigned char PhaPol_of_Spi[COUNT_OF_SPI];  //Upd - 1
 unsigned char prescaller_of_Spi[COUNT_OF_SPI];
} EE_settings;

 /* Global structure located in FLASH */
  struct ram_settings_structure {
 unsigned int baud_of_Uart[COUNT_OF_UARTS];
 unsigned char MODE_of_Uart[COUNT_OF_UARTS];
 unsigned char MODE_of_Spi[COUNT_OF_SPI]; // bits 0 - master/slave, 1 - phase, 2 - polarity
 unsigned char PhaPol_of_Spi[COUNT_OF_SPI];  //Upd - 1
 unsigned char prescaller_of_Spi[COUNT_OF_SPI];
} RAM_settings;

//ram_structure i;

#endif