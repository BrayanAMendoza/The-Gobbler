//
// COMP-GENG 421 - Tom Lupfer
//
// Session 26 - Gobbler Game

#include "main.h"
#include "accel.h"
#include "clock.h"
#include "dbg.h"
#include "gobbler.h"
#include "i2c.h"
#include "lcd.h"
#include "nav.h"
#include "rgb.h"
#include "spi.h"
#include "uart.h"


int main(void)
{
	/// Initialize the MCU hardware required for debugging first
	ClockInit();
	DbgInit();
	UartInit();

	// Initialize the remaining MCU hardware
	I2cInit();
	SpiInit();

	// Initialize the external peripherals
	AccelInit();
	LcdInit();
	NavInit();
	RgbInit();

	// Main loop
	while(TRUE)
	{
		// Wait for tick timer
		ClockWaitForTick();

		// Clock the state machines
		DBG_SET();
		NavClock();
		RgbClock();
		GobblerClock();
		DBG_CLR();
	}	// End - main loop
}
