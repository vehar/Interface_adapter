#ifndef I2C_AT24C_EEP_H
#define I2C_AT24C_EEP_H


#include <IIC_ultimate.h>
#include <avrlibtypes.h>
#include <avrlibdefs.h>


extern u08 i2c_eep_WriteByte(u08 SAddr,u16 Addr, u08 Byte, IIC_F WhatDo);
extern u08 i2c_eep_ReadByte(u08 SAddr, u16 Addr, u08 ByteNumber, IIC_F WhatDo);

#endif
