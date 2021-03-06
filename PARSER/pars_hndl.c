//���������� ������ ������

#include <adapter.h>

//#include "RTOS/HAL.h"
//#include "RTOS/EERTOS.c"
#include "RTOS/EERTOSHAL.h"

void PARS_Handler(uint8_t argc, char *argv[])
{
char __flash *response = error;
 // uint8_t value = 0;
 // uint8_t mode = 0;
  uint8_t i = 0;

 uint8_t Interface_Num = 0;
 uint32_t tmp = 0;
 bool Tmp_param_1 = 0;
 bool Tmp_param_2 = 0;

char str [6];

#ifdef DEBUG
//StopRTOS();
#endif
/////////////////////SET_COMAND////////////////////////////////
/////////////////////////////////////////////////////////////////
if (!strcmpf(argv[0], Set))
 {
#ifdef DEBUG
 Put_In_Log("\r Set");
#endif
   if (argc > 1)
  {
//////////////////////////////////////////////////////////////////
/////////////////////UART_SET_START///////////////////////////////
      if (!strcmpf(argv[1], Uart))
     {
#ifdef DEBUG
   Put_In_Log("\r Uart");
#endif

       if (argc > 2)
        {
          tmp = PARS_StrToUint(argv[2]);//Get number of interface
          if (tmp <= COUNT_OF_UARTS){Interface_Num = tmp; response = ok;
#ifdef DEBUG
  Put_In_Log("\r Num");
#endif
          }
          else{response = largeValue; goto exit;}

         if (argc > 3)    //Mode
        {
             if (!strcmpf(argv[3], Mode))
             {
               tmp = PARS_StrToUchar(argv[4]); //Get uart mode
              if (tmp==1 || tmp==0){RAM_settings.MODE_of_Uart[Interface_Num] = tmp; response = ok;
#ifdef DEBUG
  Put_In_Log("\r Mode");
#endif
               i = 2; //go to next param "speed"
               }
              else{response = largeValue;
#ifdef DEBUG
 //USART_Send_Str(SYSTEM_USART,"\rM EXIT");
 Put_In_Log("\rM EXIT");
#endif
            goto exit;}
             }

             if (!strcmpf(argv[3+i], Speed)) //may be 3 or 5th param
             {
              tmp = PARS_StrToUint(argv[4+i]); //get Baud Rate
              if (tmp <= MAX_BAUD_RATE)
              {
              RAM_settings.baud_of_Uart[Interface_Num] = tmp;
              response = ok; i = 0;
#ifdef DEBUG
  Put_In_Log("\r ");
  Put_In_LogFl(Speed);
#endif
              }
              else{response = largeValue;
#ifdef DEBUG
 Put_In_Log("\rM EXIT");
#endif
             goto exit;}
        }

     USART_Init(Interface_Num, RAM_settings.MODE_of_Uart[Interface_Num], RAM_settings.baud_of_Uart[Interface_Num]);
     Interface_Num = 0;
       
     StopRTOS();
     EE_settings.MODE_of_Uart[Interface_Num] = RAM_settings.MODE_of_Uart[Interface_Num];  //save to eeprom
     EE_settings.baud_of_Uart[Interface_Num] = RAM_settings.baud_of_Uart[Interface_Num];  
     RunRTOS();
#ifdef DEBUG          //�� ����������� ���������!!!
 Put_In_Log("\r Uart_init");
 //print_settings_ram();
#endif
      }
     }
#ifdef DEBUG
       USART_FlushTxBuf(USART_0);
#endif
    }
/////////////////////UART_SET_END////////////////////////////////
/////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
/////////////////////SPI_SET_START////////////////////////////////
     if (!strcmpf(argv[1], Spi))
     {
#ifdef DEBUG
    Put_In_Log("\r Spi");
#endif
       if (argc > 2)
        {
#ifdef DEBUG
  Put_In_Log("\rS Num");
#endif
          tmp = PARS_StrToUchar(argv[2]);//Get number of interface
          if (tmp <= COUNT_OF_SPI){Interface_Num = tmp; response = ok;}
          else{response = largeValue; goto exit;}

      if (argc > 3)    //Mode
      {
         if (!strcmpf(argv[3], Mode))
         {
            tmp = PARS_StrToUchar(argv[4]); //Get spi mode
            if (tmp==1 || tmp==0){RAM_settings.MODE_of_Spi[Interface_Num] = tmp;
            i = 2; //go to next param "speed"
            response = ok;
 #ifdef DEBUG
Put_In_Log("\rS Mode-");
#endif
            }
            else{response = largeValue; goto exit;}
        }

          if (!strcmpf(argv[3+i], Prescaller)) //Prescaller, may be 3 or 5th param
         {
            tmp = PARS_StrToUchar(argv[4+i]); //get SPI prescaller Rate
            if (tmp <= MAX_SPI_PRESCALLER){RAM_settings.prescaller_of_Spi[Interface_Num] = tmp;
            i += 2;
            response = ok;
#ifdef DEBUG
Put_In_Log("\rS Presc-");
#endif
            }
            else{response = largeValue; goto exit;}
        }

          if (!strcmpf(argv[3+i], PhaPol)) //may be 3 or 5 or 7th  param
         {
            tmp = PARS_StrToUchar(argv[4+i]);//get phase/polarity mode (0-3)
           if (tmp>=0 && tmp <= 3)
           {
            switch(tmp){  //phase/polarity select
             case 0:
              Tmp_param_1 = 0; Tmp_param_2 = 0;
             break;
             case 1:
              Tmp_param_1 = 0; Tmp_param_2 = 1;
             break;
             case 2:
              Tmp_param_1 = 1; Tmp_param_2 = 0;
             break;
             case 3:
              Tmp_param_1 = 1; Tmp_param_2 = 1;
             break;
             default:
              Tmp_param_1 = 0; Tmp_param_2 = 0;
             break;
            } response = ok;
#ifdef DEBUG
 Put_In_Log("\rS PhaPol-");
#endif
            RAM_settings.PhaPol_of_Spi[Interface_Num] = tmp; //Upd - 1
           }
           else{response = wrongValue; goto exit;}
         }
     SPI_init(Interface_Num, RAM_settings.MODE_of_Spi[Interface_Num], Tmp_param_2 ,Tmp_param_1, RAM_settings.prescaller_of_Spi[Interface_Num]);
     i = 0; Tmp_param_1=0; Tmp_param_2=0;

      EE_settings.MODE_of_Spi[Interface_Num] = RAM_settings.MODE_of_Spi[Interface_Num];
      EE_settings.PhaPol_of_Spi[Interface_Num] = RAM_settings.PhaPol_of_Spi[Interface_Num];//Upd - 1
     // Tmp_param_2 ,Tmp_param_1,
      EE_settings.prescaller_of_Spi[Interface_Num] = RAM_settings.prescaller_of_Spi[Interface_Num];


#ifdef DEBUG
 Put_In_Log("\rS SpiInit");
#endif
      }
     }
    }
/////////////////////SPI_SET_END//////////////////////////////////
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
/////////////////////I2C_SET_START///////////////////////////////
      if (!strcmpf(argv[1], I2c))
     {
#ifdef DEBUG
   Put_In_Log("\r I2c");
#endif

       if (argc > 2)
        {
          tmp = PARS_StrToUint(argv[2]);//Get number of interface
          if (tmp <= COUNT_OF_UARTS){Interface_Num = tmp; response = ok;
#ifdef DEBUG
  Put_In_Log("\r Num");
#endif
          }
          else{response = largeValue; goto exit;}

         if (argc > 3)    //Mode
        {
             if (!strcmpf(argv[3], Mode))
             {
               tmp = PARS_StrToUchar(argv[4]); //Get uart mode
              if (tmp==1 || tmp==0){RAM_settings.MODE_of_Uart[Interface_Num] = tmp; response = ok;
#ifdef DEBUG
  Put_In_Log("\r Mode");
#endif
               i = 2; //go to next param "speed"
               }
              else{response = largeValue;
#ifdef DEBUG
 //USART_Send_Str(SYSTEM_USART,"\rM EXIT");
 Put_In_Log("\rM EXIT");
#endif
            goto exit;}
             }

             if (!strcmpf(argv[3+i], Speed)) //may be 3 or 5th param
             {
              tmp = PARS_StrToUint(argv[4+i]); //get Baud Rate
              if (tmp <= MAX_BAUD_RATE)
              {
              RAM_settings.baud_of_Uart[Interface_Num] = tmp;
              response = ok; i = 0;
#ifdef DEBUG
  Put_In_Log("\r ");
  Put_In_LogFl(Speed);
#endif
              }
              else{response = largeValue;
#ifdef DEBUG
 Put_In_Log("\rM EXIT");
#endif
             goto exit;}
        }

     USART_Init(Interface_Num, RAM_settings.MODE_of_Uart[Interface_Num], RAM_settings.baud_of_Uart[Interface_Num]);
     Interface_Num = 0; //?

     EE_settings.MODE_of_Uart[Interface_Num] = RAM_settings.MODE_of_Uart[Interface_Num];  //save to eeprom
     EE_settings.baud_of_Uart[Interface_Num] = RAM_settings.baud_of_Uart[Interface_Num];
#ifdef DEBUG          //�� ����������� ���������!!!
 Put_In_Log("\r I2c_init");
 //print_settings_ram();
#endif
      }
     }
#ifdef DEBUG
       USART_FlushTxBuf(USART_0);
#endif
    }
/////////////////////I2C_SET_END////////////////////////////////
/////////////////////////////////////////////////////////////////
  }
 }

/////////////////////WRITE_COMAND////////////////////////////////
/////////////////////////////////////////////////////////////////
if (!strcmpf(argv[0], W)) //Write
 {
 #ifdef DEBUG
 Put_In_Log("\r W");
#endif
  if (argc > 1)
  {
///////////////////////////////////////////////////////////////////
/////////////////////UART_WRITE_START//////////////////////////////
      if (!strcmpf(argv[1], Uart))
     {
       if (argc > 2)
        {
          tmp = PARS_StrToUchar(argv[2]);//Get number of interface to write
          if (tmp <= COUNT_OF_UARTS){Interface_Num = tmp; response = ok;}
          else{response = largeValue;}
      if (argc > 3)    //Data
      {
      StopRTOS; //��� ��� �������� �� UART-� ����� ���� ���������,RTOS �� ����� ����������� (��� ������� ��������� ������������)
        USART_Send_Str(Interface_Num, argv[3]); //TODO ��������� �������� ��������
      RunRTOS;
      #ifdef DEBUG
Put_In_Log("\r U D>TX ");
  itoa(PARS_StrToUint(argv[3]),str);
Put_In_Log(str); //convert dec to str
     #endif
      }
     }
    }
/////////////////////UART_WRITE_END////////////////////////////////
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
/////////////////////SPI_WRITE-READ_START//////////////////////////
    if (!strcmpf(argv[1], Spi))
     {
 #ifdef DEBUG
 //USART_Send_Str(SYSTEM_USART,"\r SPI ");
 #endif
       if (argc > 2)
        {
          tmp = PARS_StrToUchar(argv[2]);//Get number of interface to write
          if (tmp <= COUNT_OF_SPI){Interface_Num = tmp; response = ok;
 #ifdef DEBUG
 //USART_Send_Str(SYSTEM_USART,"\r SPI num w ");
 #endif
          }
          else{response = largeValue;}

      if (argc > 3)    //Data
      {
#ifdef DEBUG

Put_In_Log("\r S D>TX ");
 itoa(PARS_StrToUint(argv[3]),str);  //�������� ������� �� > 65535
Put_In_Log(str); //convert dec to str
 #endif

 SPI_RW_Buf(10/*array_size(argv[3])*/, argv[3], Spi0_RX_buf); //TODO ���������� ���-�� �������������

 #ifdef DEBUG
Put_In_Log("\r S D<RX ");
Put_In_Log(Spi0_RX_buf);

#warning ������ �� ������, � ������ ���� ����, ��� ���� ���������� ������ ������� �������
         //���� ����������� � ������� ������ � � ������������ ���������� ������ ��� �������� �����
FLAG_SET(g_tcf,S_SPI_BUF_CLR);//  = SetTask(Task_SPI_ClrBuf);
//SetTimerTask(Task_SPI_ClrBuf, 10);
 // USART_Send_Str(SYSTEM_USART, Spi0_RX_buf);
  #endif
      }
     }
    }
/////////////////////SPI_WRITE-READ_END////////////////////////////
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
/////////////////////I2C_WRITE_START//////////////////////////////
    if (!strcmpf(argv[1], I2c))
     {
 #ifdef DEBUG
 //USART_Send_Str(SYSTEM_USART,"\r I2c ");
 #endif
       if (argc > 2)
        {
          tmp = PARS_StrToUchar(argv[2]);//Get number of interface to write
          if (tmp <= COUNT_OF_I2C){Interface_Num = tmp; response = ok;
 #ifdef DEBUG
 //USART_Send_Str(SYSTEM_USART,"\r I2C num w ");
 #endif
          }
          else{response = largeValue;}

      if (argc > 3)    //Data
      {
#ifdef DEBUG

Put_In_Log("\r I2C D>TX ");
 itoa(PARS_StrToUint(argv[3]),str);  //�������� ������� �� > 65535
 USART_Send_Str(SYSTEM_USART,str); //convert dec to str
 #endif

 //I2C_RW_Buf(10/*array_size(argv[3])*/, argv[3], Spi0_RX_buf); //TODO ���������� ���-�� �������������
 //I2C_write(argv[3]);


 #ifdef DEBUG
Put_In_Log("\r I2C D<RX ");
  USART_Send_Str(SYSTEM_USART, Spi0_RX_buf);
  #endif
      }
     }
    }
/////////////////////I2C_WRITE_END/////////////////////////////////
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
/////////////////////READ_COMAND_START//////////////////////////////
 


/////////////////////READ_COMAND_END//////////////////////////////
///////////////////////////////////////////////////////////////////
   }
 }

 /*
 ���������  ���� � uart1,
 �������� ��������� spi (� 1 ����������!!).
 �������� ���������� � ������,

 ������� �2�,
 �������� ����� ������������ �/�� �������� ���������.
 */
      
if (!strcmpf(argv[0], "y")){  SetTask(Task_BuffOut);  U1_in_buf_flag = 1; response = ok;}
if (!strcmpf(argv[0], "n")){  ClearTimerTask(Task_BuffOut);  U1_in_buf_flag = 0; response = ok;}
if (!strcmpf(argv[0], "a")){  SetTask(Task_AdcOnLcd);  response = ok;}
 
    if (!strcmpf(argv[0], Help)){ print_help(); response = ok; }
    if (!strcmpf(argv[0], boot)){response = ok;#asm("call 0x1E00");}//Boot_reset "Goto bootloader"
    if (!strcmpf(argv[0], reset)){response = ok;#asm("jmp 0x0000");} //reset
    // dbg �� �������� ��������� ��-�� ���������� ����(�������� �������� �������������, � � help �� ��.)
    if (!strcmpf(argv[0], dbg)){StopRTOS(); print_settings_eeprom(); print_settings_ram(); print_sys(); response = ok; RunRTOS();}//stop=15kbt
    if (!strcmpf(argv[0], "s")){ print_sys(); response = ok;}
    if (!strcmpf(argv[0], "E")){SetTask(EEP_StartWrite); response = ok;}  // ��������� ������� ������ � ������.

 //EE_settings = RAM_settings; //rewrite settings to EEPROM

exit:
  //USART_FlushTxBuf(SYSTEM_USART);
  USART_Send_StrFl(SYSTEM_USART,response);
#ifdef DEBUG
 // RunRTOS();
#endif
}

