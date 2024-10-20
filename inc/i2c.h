//
// COMP-GENG 421 - Tom Lupfer
//
// I2C module header file
//

#ifndef I2C_H__
#define I2C_H__

void I2cInit(void);
void I2cWrite(UInt8 DevAddr, UInt8 RegAddr, int NumBytes, UInt8 * DataBuf);
void I2cRead(UInt8 DevAddr, UInt8 RegAddr, int NumBytes, UInt8 * DataBuf);

#endif // I2C_H__
