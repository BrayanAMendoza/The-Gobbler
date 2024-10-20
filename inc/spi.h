//
// COMP-GENG 421 - Tom Lupfer
//
// SPI module header file
//

#ifndef SPI_H_
#define SPI_H_

void SpiInit(void);
void SpiCmdWrite(UInt8 Val);
void SpiDataWrite(UInt8 Val);
void SpiResetAssert(void);
void SpiResetDeassert(void);

#endif	// SPI_H_
