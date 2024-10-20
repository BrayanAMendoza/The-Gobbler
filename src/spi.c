//
// COMP-GENG 421 - Tom Lupfer
//
// SPI module
//

#include "main.h"
#include "spi.h"


// Software SPI interface signal definitions
#define SCK_CLR()		(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5))
#define SCK_SET()		(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5))
#define RST_DIS()		(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_6))
#define RST_ENA()		(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6))
#define MOSI_CLR()		(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7))
#define MOSI_SET()		(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_7))
#define CMD_ENA()		(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8))
#define DAT_ENA()		(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_8))
#define CS_DIS()		(LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6))
#define CS_ENA()		(LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6))


void SpiInit(void)
{
	// Enable the clocks for the PAx, PBx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	// PA5 -> LCD SCK (MOSI valid on rising edge)
	// Init PA5 high, configure as an output
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);

	// PA6 -> LCD RESET# (active low)
	// Init PA6 low, configure as an output
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_6);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);

	// PA7 -> LCD MOSI (active high)
	// Init PA7 low, configure as an output
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_7);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);

	// PA8 -> LCD A0 - 0: command, 1: data
	// Init PA8 low, configure as an output
	LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_8);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_8, LL_GPIO_MODE_OUTPUT);

	// PB6 -> LCD CS# (active low)
	// Init PB6 high, configure as an output
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
}


void SpiCmdWrite(UInt8 Val)
{
	UInt BitMask;

	CMD_ENA();	// configure LCD A0 for a command

	CS_ENA();	// assert the LCD SPI chip select

	// Write each bit of the command byte with the rising edge of SCK, msb first
	for (BitMask = 0x80; BitMask > 0; BitMask >>= 1)
	{
		SCK_CLR();
		Val & BitMask ? MOSI_SET() : MOSI_CLR();
		SCK_SET();
	}

	CS_DIS();	// deassert the LCD SPI chip select
}


void SpiDataWrite(UInt8 Val)
{
	UInt BitMask;

	DAT_ENA();	// configure LCD A0 for data

	CS_ENA();	// assert the LCD SPI chip select

	// Write each bit of the data byte with the rising edge of SCK, msb first
	for (BitMask = 0x80; BitMask > 0; BitMask >>= 1)
	{
		SCK_CLR();
		Val & BitMask ? MOSI_SET() : MOSI_CLR();
		SCK_SET();
	}

	CS_DIS();	// deassert the LCD SPI chip select
}


void SpiResetAssert(void)
{
	RST_ENA();
}


void SpiResetDeassert(void)
{
	RST_DIS();
}
