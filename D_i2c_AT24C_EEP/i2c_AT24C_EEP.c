#include "D_i2c_AT24C_EEP/i2c_AT24C_EEP.h"


#define HI(X) (X>>8)
#define LO(X) (X & 0xFF)

uint8_t i2c_eep_WriteByte(uint8_t SAddr,uint16_t Addr, uint8_t Byte, IIC_F WhatDo)
{

if (i2c_Do & i2c_Busy) return 0;

i2c_index = 0;
i2c_ByteCount = 3;

i2c_SlaveAddress = SAddr;


i2c_Buffer[0] = HI(Addr);
i2c_Buffer[1] = LO(Addr);
i2c_Buffer[2] = Byte;

i2c_Do = i2c_sawp;

MasterOutFunc = WhatDo;
ErrorOutFunc = WhatDo;

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;

i2c_Do |= i2c_Busy;

return 1;
}


uint8_t i2c_eep_ReadByte(uint8_t SAddr, uint16_t Addr, uint8_t ByteNumber, IIC_F WhatDo)
{
if (i2c_Do & i2c_Busy) return 0;

i2c_index = 0;
i2c_ByteCount = ByteNumber;

i2c_SlaveAddress = SAddr;

i2c_PageAddress[0] = HI(Addr);
i2c_PageAddress[1] = LO(Addr);

i2c_PageAddrIndex = 0;
i2c_PageAddrCount = 2;

i2c_Do = i2c_sawsarp;

MasterOutFunc = WhatDo;
ErrorOutFunc = WhatDo;

TWCR = 1<<TWSTA|0<<TWSTO|1<<TWINT|0<<TWEA|1<<TWEN|1<<TWIE;

i2c_Do |= i2c_Busy;

return 1;
}
