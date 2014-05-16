//*****************************************************************************
//* BootLoader 7.4
//*
//* Devices supported at this time and report Ok, from users
//* ATMega8
//* ATMega16
//* ATMega32
//* ATMega64
//* ATMega128
//* ATMega162
//* ATMega169
//* ATMega8515
//* ATMega8535
//* ATMega88    
//* ATMega1280  
//* ATMega2560  
//* ATMCAN128   
//* ATMega164/324/644
//* ATMega324  
//* ATMega324P 
//* ATMega2561
//* ATMega164
//* ATMCAN32
//* ATMega328
//*
//* Everything is AS IS without any warranty of any kind.
//*
//* Note:
//* -----
//* I can't write bootloader of all the MCU it's not my primary job and I don't
//* make $$$ with that
//*
//* If you make new #define please let me know I will update the bootloader
//* file it will help other AVR users
//*
//* bibi@MicroSyl.com
//*****************************************************************************


//*****************************************************************************
//*****************************************************************************
// IF YOU MAKE NEW DEFINE THAT IS WORKING PLEASE LET ME KNOW TO UPDATE MEGALOAD
// This software is free, so I can't pass all my time writting new bootloader
// for new MCU. I'm shure that you can help me and help ALL MEGALOAD USERS
//*****************************************************************************
//*****************************************************************************


//*****************************************************************************
//
// To setup the bootloader for your project you must
// remove the comment below to fit with your hardware
// recompile it using ICCAVR setup for bootloader
//
// Flash, EEprom, Lockbit Programming take a bootloader of 512 word 
//
// if you chose the SMALL256 you will only be able to program the flash without
// any communication and flash verification.  You will need a bootloader size
// of 256 word
//
//*****************************************************************************
// MCU selection
//
// *************************************
// *->Do the same thing in assembly.s<-*
// *************************************
//
//*****************************************************************************

//#define MEGATYPE  Mega8         
#define MEGATYPE Mega16        
//#define MEGATYPE Mega64        
//#define MEGATYPE Mega128       
//#define MEGATYPE Mega32        
//#define MEGATYPE Mega162       
//#define MEGATYPE Mega169       
//#define MEGATYPE Mega8515      
//#define MEGATYPE Mega8535      
//#define MEGATYPE Mega163       
//#define MEGATYPE Mega323       
//#define MEGATYPE Mega48        
//#define MEGATYPE Mega88        
//#define MEGATYPE Mega168       
//#define MEGATYPE Mega165       
//#define MEGATYPE Mega3250      
//#define MEGATYPE Mega6450      
//#define MEGATYPE Mega3290      
//#define MEGATYPE Mega6490      
//#define MEGATYPE Mega406       
//#define MEGATYPE Mega640       
//#define MEGATYPE Mega1280      
//#define MEGATYPE Mega2560      
//#define MEGATYPE MCAN128       
//#define MEGATYPE Mega164				
//#define MEGATYPE Mega328				
//#define MEGATYPE Mega324				
//#define MEGATYPE Mega325				
//#define MEGATYPE Mega644				
//#define MEGATYPE Mega645				
//#define MEGATYPE Mega1281				
//#define MEGATYPE Mega2561				
//#define MEGATYPE Mega404				
//#define MEGATYPE MUSB1286				
//#define MEGATYPE MUSB1287				
//#define MEGATYPE MUSB162					
//#define MEGATYPE MUSB646					
//#define MEGATYPE MUSB647					
//#define MEGATYPE MUSB82					
//#define MEGATYPE MCAN32					
//#define MEGATYPE MCAN64			
//#define MEGATYPE Mega329
//#define MEGATYPE Mega649		
//#define MEGATYPE Mega256		
                    
//*****************************************************************************
// MCU Frequency    
//*****************************************************************************
#define XTAL        8000000
                    
//*****************************************************************************
// Bootload on UART x
//*****************************************************************************
#define UART        0
//#define UART       1
//#define UART       2
//#define UART       3

//*****************************************************************************
// BaudRate
//*****************************************************************************
#define BAUDRATE     38400

//*****************************************************************************
// EEprom programming
// enable EEprom programing via bootloader
//*****************************************************************************
//#define EEPROM

//*****************************************************************************
// LockBit programming
// enable LOCKBIT programing via bootloader
//*****************************************************************************
//#define LOCKBIT

//*****************************************************************************
// Small 256 Bootloader without eeprom programming, lockbit programming
// and no data verification
//*****************************************************************************
#define SMALL256

//*****************************************************************************
// RS485
// if you use RS485 half duplex for bootloader
// make the appropriate change for RX/TX transceiver switch
//*****************************************************************************
//#define RS485DDR  DDRB
//#define RS485PORT PORTB
//#define RS485TXE  0x08

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                 DO NOT CHANGE ANYTHING BELOW THIS LINE 
//               IF YOU DON'T REALLY KNOW WHAT YOU ARE DOING
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define WDR() #asm("wdr")

#define Mega8           'A'
#define Mega16          'B'
#define Mega64          'C'
#define Mega128         'D'
#define Mega32          'E'
#define Mega162         'F'
#define Mega169         'G'
#define Mega8515        'H'
#define Mega8535        'I'
#define Mega163         'J'
#define Mega323         'K'
#define Mega48          'L'
#define Mega88          'M'
#define Mega168         'N'

#define Mega165         0x80
#define Mega3250        0x81
#define Mega6450        0x82
#define Mega3290        0x83
#define Mega6490        0x84
#define Mega406         0x85
#define Mega640         0x86
#define Mega1280        0x87
#define Mega2560        0x88
#define MCAN128         0x89
#define Mega164					0x8a
#define Mega328					0x8b
#define Mega324					0x8c
#define Mega325					0x8d
#define Mega644					0x8e
#define Mega645					0x8f
#define Mega1281				0x90
#define Mega2561				0x91
#define Mega404					0x92
#define MUSB1286				0x93
#define MUSB1287				0x94
#define MUSB162					0x95
#define MUSB646					0x96
#define MUSB647					0x97
#define MUSB82					0x98
#define MCAN32					0x9a
#define MCAN64					0x9b
#define Mega329					0x9c
#define Mega649					0x9d
#define Mega256					0x9e

#define Flash1k         'g'
#define Flash2k         'h'
#define Flash4k         'i'
#define Flash8k         'l'
#define Flash16k        'm'
#define Flash32k        'n'
#define Flash64k        'o'
#define Flash128k       'p'
#define Flash256k       'q'
#define Flash40k        'r'

#define EEprom64        '.'
#define EEprom128       '/'
#define EEprom256       '0'
#define EEprom512       '1'
#define EEprom1024      '2'
#define EEprom2048      '3'
#define EEprom4096      '4'

#define Boot128         'a'
#define Boot256         'b'
#define Boot512         'c'
#define Boot1024        'd'
#define Boot2048        'e'
#define Boot4096        'f'

#define Page32          'Q'
#define Page64          'R'
#define Page128         'S'
#define Page256         'T'
#define Page512         'V'

#if !(defined MEGATYPE) && !(defined MCU)
  #error "Processor Type is Undefined"
#endif

#ifdef EEPROM
  #define  BootSize       Boot1024
#endif

#ifndef EEPROM
  #define  BootSize       Boot512
#endif

#if (MEGATYPE == Mega8)
  #include "iom8v.h"
  #define  DeviceID       Mega8
  #define  FlashSize      Flash8k
  #define  PageSize       Page64
  #define  EEpromSize     EEprom512
  #define  PageByte       64
  #define  NSHIFTPAGE     6
  #define  INTVECREG      GICR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega16)
 // #include "iom16v.h"   
   #include <mega16.h>
  #define  DeviceID       Mega16
  #define  FlashSize      Flash16k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom512
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      GICR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega64)
  #include "iom64v.h"
  #define  DeviceID       Mega64
  #define  FlashSize      Flash64k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom2048
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif
#endif

#if (MEGATYPE == Mega128)
  #include "iom128v.h"
  #define  DeviceID       Mega128
  #define  FlashSize      Flash128k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  RAMPZ_FLAG
  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif
#endif

#if (MEGATYPE == Mega32)
  #include "iom32v.h"
  #define  DeviceID       Mega32
  #define  FlashSize      Flash32k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom1024
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      GICR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega162)
  #include "iom162v.h"
  #define  DeviceID       Mega162
  #define  FlashSize      Flash16k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom512
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      GICR
  #if (UART == 0)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTB
    #define PULLUPPIN       0x04
  #endif
#endif

#if (MEGATYPE == Mega169)
  #include "iom169v.h"
  #define  DeviceID       Mega169
  #define  FlashSize      Flash16k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom512
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTE
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega8515)
  #include "iom8515v.h"
  #define  DeviceID       Mega8515
  #define  FlashSize      Flash8k
  #define  PageSize       Page64
  #define  EEpromSize     EEprom512
  #define  PageByte       64
  #define  NSHIFTPAGE     6
  #define  INTVECREG      GICR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega8535)
  #include "iom8515v.h"
  #define  DeviceID       Mega8535
  #define  FlashSize      Flash8k
  #define  PageSize       Page64
  #define  EEpromSize     EEprom512
  #define  PageByte       64
  #define  NSHIFTPAGE     6
  #define  INTVECREG      GICR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega163)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega323)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega48)
  #include "iom48v.h"
  #define  DeviceID       Mega48
  #define  FlashSize      Flash4k
  #define  PageSize       Page64
  #define  EEpromSize     EEprom256
  #define  PageByte       64
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega88)
  #include "iom88v.h"
  #define  DeviceID       Mega88
  #define  FlashSize      Flash8k
  #define  PageSize       Page64
  #define  EEpromSize     EEprom512
  #define  PageByte       64
  #define  NSHIFTPAGE     6
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega168)
  #include "iom168v.h"
  #define  DeviceID       Mega168
  #define  FlashSize      Flash16k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom512
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega165)
  #include "iom165v.h"
  #define  DeviceID       Mega165
  #define  FlashSize      Flash16k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom512
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTE
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega3250)
  #include "iom325v.h"
  #define  DeviceID       Mega3250
  #define  FlashSize      Flash32k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom1024
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTE
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega6450)
  #include "iom645v.h"
  #define  DeviceID       Mega6450
  #define  FlashSize      Flash64k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom2048
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTE
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega3290)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega6490)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega406)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega640)
  #include "iom640v.h"
  #define  DeviceID       Mega640
  #define  FlashSize      Flash64k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif

  #if (UART == 2)
   #define PULLUPPORT      PORTH
   #define PULLUPPIN       0x01
  #endif

  #if (UART == 3)
    #define PULLUPPORT      PORTJ
    #define PULLUPPIN       0x01
  #endif
#endif

#if (MEGATYPE == Mega1280)
  #include "iom1280v.h"
  #define  DeviceID       Mega1280
  #define  FlashSize      Flash128k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR

  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif

  #if (UART == 2)
    #define PULLUPPORT      PORTH
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 3)
    #define PULLUPPORT      PORTJ
    #define PULLUPPIN       0x01
  #endif
#endif

#if (MEGATYPE == Mega2560)
  #include "iom256v.h"
  #define  DeviceID       Mega2560
  #define  FlashSize      Flash256k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  RAMPZ_FLAG


  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif

  #if (UART == 2)
    #define PULLUPPORT      PORTH
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 3)
    #define PULLUPPORT      PORTJ
    #define PULLUPPIN       0x01
  #endif
#endif

#if (MEGATYPE == MCAN128)
  #include "ioCAN128v.h"
  #define  DeviceID       MCAN128
  #define  FlashSize      Flash128k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  RAMPZ_FLAG
 
  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif
#endif

#if (MEGATYPE == Mega164)
  #include "iom164pv.h"
  #define  DeviceID       Mega164
  #define  FlashSize      Flash16k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom512
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if(MEGATYPE == Mega324)
#include "iom324v.h"
#define  DeviceID  					Mega324
#define  FlashSize      		Flash32k
#define  PageSize       		Page128
#define  EEpromSize     		EEprom1024
#define  PageBye       			128
#define  NSHIFTPAGE     		7
#define  INTVECREG      		MCUCR
#if (UART == 0)
  #define PULLUPPORT      	PORTD
  #define PULLUPPIN       	0x01
#endif
#endif

#if (MEGATYPE == Mega325)
 #error "This MCU had not been define"
#endif

#if(MEGATYPE == Mega644)
	#include "iom644v.h"
	#define DeviceID          Mega644
	#define FlashSize         Flash64k
	#define PageSize          Page256
	#define EEpromSize        EEprom2048
	#define PageByte          256
	#define NSHIFTPAGE      	8
	#define INTVECREG        	MCUCR
	#define PULLUPPORT    		PORTD
	#define PULLUPPIN        	0x01
#endif

#if (MEGATYPE == Mega328)
  #include "iom328pv.h"
  #define  DeviceID       Mega328
  #define  FlashSize      Flash32k
  #define  PageSize       Page128
  #define  EEpromSize     EEprom1024
  #define  PageByte       128
  #define  NSHIFTPAGE     7
  #define  INTVECREG      MCUCR
  #define  PULLUPPORT     PORTD
  #define  PULLUPPIN      0x01
#endif

#if (MEGATYPE == Mega645)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega1281)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega2561)
  #include "Mega2561.h"
  #define  DeviceID       Mega2561
  #define  FlashSize      Flash256k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  IVCE           0
  #define  RAMPZ_FLAG


  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif

#endif

#if (MEGATYPE == Mega404)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MUSB1286)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MUSB1287)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MUSB162)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MUSB646)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MUSB647)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MUSB82)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == MCAN32)
  #include "ioCAN32v.h"
  #define  DeviceID       MCAN32
  #define  FlashSize      Flash32k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom1024
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  RAMPZ_FLAG
#endif

#if (MEGATYPE == MCAN64)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega329)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega649)
 #error "This MCU had not been define"
#endif

#if (MEGATYPE == Mega256)
  #include "iom256v.h"
  #define  DeviceID       Mega256
  #define  FlashSize      Flash256k
  #define  PageSize       Page256
  #define  EEpromSize     EEprom4096
  #define  PageByte       256
  #define  NSHIFTPAGE     8
  #define  INTVECREG      MCUCR
  #define  RAMPZ_FLAG


  #if (UART == 0)
    #define PULLUPPORT      PORTE
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 1)
    #define PULLUPPORT      PORTD
    #define PULLUPPIN       0x04
  #endif

  #if (UART == 2)
    #define PULLUPPORT      PORTH
    #define PULLUPPIN       0x01
  #endif

  #if (UART == 3)
    #define PULLUPPORT      PORTJ
    #define PULLUPPIN       0x01
  #endif
#endif


// Serial Port defenition

#if !(defined MEGATYPE) && !(defined MCU)
  #error "Processor Type is Undefined"
#endif

#if (UART == 0) && !(defined UCSR0A)
 #define  _UCSRA          UCSRA
 #define  _UCSRB         	UCSRB
 #define  _UCSRC         	UCSRC
 #define  _UBRRL          UBRRL
 #define  _UBRRH          UBRRH
 #define  _UDR            UDR
 #define  _TXC						TXC
#endif

#if (UART == 0) && (defined UCSR0A)
 #define  _UCSRA          UCSR0A
 #define  _UCSRB         	UCSR0B
 #define  _UCSRC         	UCSR0C
 #define  _UBRRL          UBRR0L
 #define  _UBRRH          UBRR0H
 #define  _UDR            UDR0
 #define  _TXC						TXC0 
#endif

#if (UART == 1)
 #define  _UCSRA          UCSR1A
 #define  _UCSRB         	UCSR1B
 #define  _UCSRC         	UCSR1C
 #define  _UBRRL          UBRR1L
 #define  _UBRRH          UBRR1H
 #define  _UDR            UDR1
 #define  _TXC						TXC1 
#endif

#if (UART == 2)
 #define  _UCSRA          UCSR2A
 #define  _UCSRB         	UCSR2B
 #define  _UCSRC         	UCSR2C
 #define  _UBRRL          UBRR2L
 #define  _UBRRH          UBRR2H
 #define  _UDR            UDR2
 #define  _TXC						TX2 
#endif

#if (UART == 3)
 #define  _UCSRA          UCSR3A
 #define  _UCSRB         	UCSR3B
 #define  _UCSRC         	UCSR3C
 #define  _UBRRL          UBRR3L
 #define  _UBRRH          UBRR3H
 #define  _UDR            UDR3
 #define  _TXC						TXC3 
#endif

#define  FALSE          0
#define  TRUE           1

#ifdef SMALL256
 #undef EEPROM
 #undef LOCKBIT
#endif


/*****************************************************************************/
/*                          I N C L U D E                                    */
/*****************************************************************************/

/*****************************************************************************/
/*                        P R O T O T Y P E                                  */
/*****************************************************************************/

void GetPageNumber(void);
char WriteFlashPage(void);

unsigned char RxChar(void);
void TxChar(unsigned char ch);

#ifdef EEPROM
void EEpromLoad(void);
void EEPROMwrite(int location, unsigned char byte);
unsigned char EEPROMread( int location);
void LockBit(void);
#endif

void main(void);

/*****************************************************************************/
/*                G L O B A L    V A R I A B L E S                           */
/*****************************************************************************/
//unsigned char PageBuffer[PageByte];
unsigned int PageAddress;
unsigned int RealPageAddress;  

#if ((MEGATYPE == Mega64) || (MEGATYPE == Mega128))
 #asm(".EQU SPMCR = 0x68")
#elif((MEGATYPE == Mega324) || (MEGATYPE == Mega644))
 #asm(".EQU SPMCR = 0x37")
#else
 #asm(".EQU SPMCR = 0x57")
#endif

#pragma warn-
#pragma used+
void WAIT_SPMEN(void)
{
#asm
        LDS     R30,SPMCR       ; load SPMCR to R27
        SBRC    R30,0          ; check SPMEN flag
        RJMP    _WAIT_SPMEN     ; wait for SPMEN flag cleared        
#endasm
}
void write_page (unsigned int adr, unsigned char function)
//; bits 8:15 adr addresses the page...(must setup RAMPZ beforehand!!!)
{
#asm
        CALL _WAIT_SPMEN
        LDD R30,Y+1        
        LDD R31,Y+2         ;move address to z pointer (R31=ZH R30=ZL)
        LDD R26,Y+0
        STS SPMCR,R26       ;argument 2 decides function
        SPM                 ;perform pagewrite
#endasm
}
void fill_temp_buffer (unsigned int data, unsigned int adr)
{
//; bits 7:1 in adr addresses the word in the page... (2=first word, 4=second word etc..)
#asm
        CALL _WAIT_SPMEN
        LDD R31,Y+1        
        LDD R30,Y+0         ;move adress to z pointer (R31=ZH R30=ZL)
        LDD R1,Y+3
        LDD R0,Y+2          ;move data to reg 0 and 1      
        LDI R26,0x01
        STS SPMCR,R26
        SPM            ;Store program memory
#endasm
}
unsigned int read_program_memory (unsigned int adr ,unsigned char cmd)
{
#asm
        LDD R31,Y+2         ;R31=ZH R30=ZL
        LDD R30,Y+1         ;move adress to z pointer
        LDD R26,Y+0
        SBRC R26,0          ;read lockbits? (second argument=0x09)
        STS SPMCR,R26       ;if so, place second argument in SPMEN register  
#endasm 

#ifdef RAMPZ_FLAG
 #asm
        ELPM    r26, Z+         ;read LSB
        ELPM    r27, Z          ;read MSB
 #endasm
#else
 #asm
        LPM     r26, Z+
        LPM     r27, Z
 #endasm
#endif
#asm
        MOVW R30,R26
#endasm
}
#ifdef LOCKBIT
void write_lock_bits (unsigned char val)
{
#asm
        LD  R0,Y   
        LDI R30,0x09     
        STS SPMCR,R30
        SPM                ;write lockbits
#endasm
}
#endif
void enableRWW(void)
{
#asm
        CALL _WAIT_SPMEN
        LDI R27,0x11
        STS SPMCR,R27
        SPM
#endasm
}
#pragma used-
#pragma warn+ 

/*****************************************************************************/

void GetPageNumber(void)
{
  unsigned char PageAddressHigh = RxChar();

  RealPageAddress = (((unsigned int)PageAddressHigh << 8) + RxChar());
  PageAddress = RealPageAddress << NSHIFTPAGE;

  #ifdef RAMPZ_FLAG
  RAMPZ = PageAddressHigh;
  #endif
}

/*****************************************************************************/

char WriteFlashPage(void)
{
	#ifdef SMALL256
  //-------------
  unsigned int i;
  unsigned int TempInt;
 
  for (i=0;i<PageByte;i+=2) 
  { 
  	TempInt = RxChar();
  	TempInt |= ((unsigned int)RxChar()<<8);
  	fill_temp_buffer(TempInt,i);    //call asm routine.
  }
  write_page(PageAddress,0x03);     //Perform page ERASE
  write_page(PageAddress,0x05);     //Perform page write
  enableRWW();  	
  i = RxChar();
  return 1;
 
  #else //--------------
  
  unsigned int i;
  unsigned int TempInt;
  unsigned char FlashCheckSum = 0;
  unsigned char CheckSum = 0;
  unsigned char Left;
  unsigned char Right;
 
  for (i=0;i<PageByte;i+=2) 
  { 
   Right = RxChar();
   Left = RxChar();
   TempInt = (unsigned int)Right + ((unsigned int)Left<<8);
   CheckSum += (Right + Left);
   fill_temp_buffer(TempInt,i);      //call asm routine.
  }

  if (CheckSum != RxChar()) return 0;
 
  write_page(PageAddress,0x03);     //Perform page ERASE
  write_page(PageAddress,0x05);     //Perform page write
  enableRWW();
  for (i=0;i<PageByte;i+=2)
  {
    TempInt = read_program_memory(PageAddress + i,0x00);
    FlashCheckSum += (char)(TempInt & 0x00ff) + (char)(TempInt >> 8);
  }
  if (CheckSum != FlashCheckSum) return 0;
  
  return 1;
  
  #endif
}

/*****************************************************************************/
/* EEprom Programing Code                                                    */
/*****************************************************************************/
#ifdef EEPROM
void EEpromLoad()
{
  unsigned char ByteAddressHigh;
  unsigned char ByteAddressLow;
  unsigned int ByteAddress;
  unsigned char Data;
  unsigned char LocalCheckSum;
  unsigned char CheckSum;

  TxChar(')');
  TxChar('!');
  while (1)
  {
  	WDR(); 
    LocalCheckSum = 0;

    ByteAddressHigh = RxChar();
    LocalCheckSum += ByteAddressHigh;

    ByteAddressLow = RxChar();
    LocalCheckSum += ByteAddressLow;

    ByteAddress = (ByteAddressHigh<<8)+ByteAddressLow;

    if (ByteAddress == 0xffff) return;

    Data = RxChar();
    LocalCheckSum += Data;

    CheckSum = RxChar();

    if (CheckSum == LocalCheckSum)
    {
      EEPROMwrite(ByteAddress, Data);
      if (EEPROMread(ByteAddress) == Data) TxChar('!');
      else TxChar('@');
    }
    else
    {
      TxChar('@');
    }
  }
}
#endif

/*****************************************************************************/

#ifdef EEPROM
void EEPROMwrite( int location, unsigned char byte)
{
  while (EECR & 0x02) WDR();        // Wait until any earlier write is done
  EEAR = location;
  EEDR = byte;
  EECR |= 0x04;                     // Set MASTER WRITE enable
  EECR |= 0x02;                     // Set WRITE strobe
}
#endif

/*****************************************************************************/

#ifdef EEPROM
unsigned char EEPROMread( int location)
{
  while (EECR & 0x02) WDR(); 
  EEAR = location;
  EECR |= 0x01;                     // Set READ strobe
  return (EEDR);                    // Return byte
}
#endif

/*****************************************************************************/
/* LockBit Code                                                              */
/*****************************************************************************/
#ifdef LOCKBIT
void LockBit(void)
{
  unsigned char Byte;

  TxChar('%');

  Byte = RxChar();

  if (Byte == ~RxChar()) write_lock_bits(~Byte);
}
#endif

/*****************************************************************************/
/* Serial Port Code                                                          */
/*****************************************************************************/

/*****************************************************************************/

unsigned char RxChar(void)
{
	unsigned int TimeOut = 0;

	while(!(_UCSRA & 0x80))
	{
		WDR(); 
		TimeOut += 2;
		TimeOut -= 1;
		if (TimeOut > 65530) break;
	}
	
  return _UDR;
}

/*****************************************************************************/

void TxChar(unsigned char ch)
{
  while(!(_UCSRA & 0x20)) WDR();      // wait for empty transmit buffer
  #ifndef RS485DDR
  _UDR = ch;                         // write char
  #endif

  #ifdef RS485DDR
  RS485PORT |= RS485TXE;            // RS485 in TX mode
  _UDR = ch;                        // write char
  while(!(_UCSRA & 0x40)) WDR();    // Wait for char to be cue off
  _UCSRA |= 0x40;                   // Clear flag
  RS485PORT &= ~RS485TXE;           // RS485 in RX mode
  #endif	
}

/*****************************************************************************/

void main(void)
{
  PULLUPPORT = PULLUPPIN;           // Pull up on RX line
  
  //_UBRRH = ((XTAL / (16 * BAUDRATE)) - 1)>>8;
 	_UBRRL = (XTAL / (16 * BAUDRATE)) - 1;      //set baud rate;
	_UCSRB = 0x18;                     // Rx enable Tx Enable
	_UCSRC = 0x86;                     // Asyn,NoParity,1StopBit,8Bit	

	#ifdef RS485DDR
	RS485DDR |= RS485TXE;             // RS485 Tranceiver switch pin as output
	RS485PORT &= ~RS485TXE;           // RS485 in Rx mode
	#endif

	RxChar();
	TxChar('>');
	if (RxChar() == '<')
	{
	    TxChar(PageSize);
		TxChar(DeviceID);
    	TxChar(FlashSize);
  	    TxChar(BootSize);
  	    TxChar(EEpromSize);
		
  	RxChar();
  	TxChar('!');

		while (1)
		{ 
		 WDR(); 
		 GetPageNumber();
		 if (RealPageAddress == 0xffff) break;

		 if (WriteFlashPage()){TxChar('!');}
		 else  break; //TxChar('@');            
		}
		
		#ifdef EEPROM
		EEpromLoad();
		#endif
		#ifdef LOCKBIT
		LockBit();
		#endif		
	}

  #ifdef RAMPZ_FLAG
  RAMPZ = 0;
  #endif

  #ifdef INTVECREG
  //MCUCR = (1<<IVCE);
 // MCUCR = 0x00;  
 //maebe: 
     //INTVECREG = (1<<0);
     //INTVECREG = 0x00; 
  #endif     
  
  #asm("jmp 0x0000");                // Run application code
}
