//---------------------START UART VARS----------------//

//---------------------END UART VARS----------------//


#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include <stdint.h>
#include "D_usart/usart.h"

/* this structure will occupy 1 byte in RAM
   as the bit field data type is unsigned char */
#warning ������ �� union_ ������������ ������ ����������
volatile struct flag_collector0 {
              unsigned char Ultra_L_Req:1; /* bit 0 */
              unsigned char Ultra_R_Req:1; /* bit 1 */
              unsigned char Ultra_L_Rdy:1; /* bit 2 */
              unsigned char Ultra_R_Rdy:1; /* bit 3 */
              unsigned char Lsm303_Req :1; /* bit 4 */
              unsigned char Lsm303_Rdy :1; /* bit 5 */
              unsigned char Parser_Req :1; /* bit 6 */ //������ �� ��������� ��������� ������
              unsigned char aa:1; /* bit 7 */
              }flags;



//volatile uint32_t result = 0;
extern volatile uint8_t symbol = 0;
extern volatile uint8_t Parser_req_state_cnt = 255;
register volatile uint32_t v_u32_SYS_TICK    = 0; //may be static

extern volatile uint32_t v_u32_TX_CNT = 0;
extern volatile uint32_t v_u32_RX_CNT = 0;


/*�������*/
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
 ��������� �������
*/
////////////////////////////////////////////////////
////////////////////////////////////////////////////

/*������*/
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
__flash char timeslot[]   = " T"; //�������/�������� 1 ����- ����
__flash char presense[]   = " Pr"; //�������/�������� 1 ����- ����
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


//�������� ���������

 #warning ��������������!
 /* Global structure located in EEPROM */
eeprom  struct eeprom_settings_structure {
 unsigned int baud_of_Uart[COUNT_OF_UARTS];
 unsigned char MODE_of_Uart[COUNT_OF_UARTS];
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