#ifndef IICULTIMATE_H
#define IICULTIMATE_H


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avrlibtypes.h>
#include <avrlibdefs.h>

#define i2c_PORT	PORTC
#define i2c_DDR		DDRC
#define i2c_SCL		0
#define i2c_SDA		1


#define i2c_MasterAddress 	0xB0
#define i2c_i_am_slave		1

#define i2c_MasterBytesRX	2			// Slave Receive buffer
#define i2c_MasterBytesTX	2			// Slave Transmitt buffer

#define i2c_MaxBuffer		3			// Max length of Master RX-TX Buffer
#define i2c_MaxPageAddrLgth	2			// Max length of Page Adress 



#define i2c_type_msk	0b00001100
#define i2c_sarp		0b00000000		// Start-Addr_R-Read-Stop
#define i2c_sawp		0b00000100		// Start-Addr_W-Write-Stop
#define i2c_sawsarp		0b00001000		// Start-Addr_W-WrPageAdr-rStart-Addr_R-Read-Stop

#define i2c_Err_msk		0b00110011
#define i2c_Err_NO		0b00000000		// All Right!
#define i2c_ERR_NA		0b00010000		// Device No Answer
#define i2c_ERR_LP		0b00100000		// Low Priority
#define i2c_ERR_NK		0b00000010		// Received NACK. End Transmittion.
#define i2c_ERR_BF		0b00000001		// BUS FAIL

#define i2c_Interrupted		0b10000000	// Transmiting Interrupted
#define i2c_NoInterrupted 	0b01111111  // Transmiting No Interrupted

#define i2c_Busy		0b01000000  	// Trans is Busy
#define i2c_Free		0b10111111  	// Trans is Free


#define MACRO_i2c_WhatDo_MasterOut 	(MasterOutFunc)();	
#define MACRO_i2c_WhatDo_SlaveOut   (SlaveOutFunc)();
#define MACRO_i2c_WhatDo_ErrorOut   (ErrorOutFunc)();



typedef void (*IIC_F)(void);

extern IIC_F MasterOutFunc;
extern IIC_F SlaveOutFunc;
extern IIC_F ErrorOutFunc;


extern u08 i2c_Do;
extern u08 i2c_InBuff[i2c_MasterBytesRX];
extern u08 i2c_OutBuff[i2c_MasterBytesTX];

extern u08 i2c_SlaveIndex;


extern u08 i2c_Buffer[i2c_MaxBuffer];
extern u08 i2c_index;
extern u08 i2c_ByteCount;

extern u08 i2c_SlaveAddress;
extern u08 i2c_PageAddress[i2c_MaxPageAddrLgth];

extern u08 i2c_PageAddrIndex;
extern u08 i2c_PageAddrCount;


extern void Init_i2c(void);
extern void Init_Slave_i2c(IIC_F Addr);

/*
extern u08 	WorkLog[100];
extern u08	WorkIndex;
*/

#endif
