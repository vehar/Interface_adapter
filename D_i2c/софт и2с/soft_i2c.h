/*
  CodeVisionAVR V2.05.1+ C Compiler
  (C) 1998-2011 Pavel Haiduc, HP InfoTech S.R.L.

  Prototypes for I2C bus master functions

  The I/O port and bits used for the SDA and
  SCL signals and Bit Rate must be specified in the
  Project|Configure|C Compiler|Libraries|I2C menu.
*/

#ifndef _I2C_INCLUDED_
#define _I2C_INCLUDED_

#pragma used+
void i2c_init(void);
unsigned char i2c_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
unsigned char i2c_write(unsigned char data);
#pragma used-

#endif

