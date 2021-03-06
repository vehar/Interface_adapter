#ifndef I2C_AT24C_EEP_H
#define I2C_AT24C_EEP_H


#include "D_IIC_ultimate/IIC_ultimate.h"
#include "D_avrlib/avrlibtypes.h"
#include "D_avrlib/avrlibdefs.h"


extern uint8_t i2c_eep_WriteByte(uint8_t SAddr,uint16_t Addr, uint8_t Byte, IIC_F WhatDo);
extern uint8_t i2c_eep_ReadByte(uint8_t SAddr, uint16_t Addr, uint8_t ByteNumber, IIC_F WhatDo);

#endif
