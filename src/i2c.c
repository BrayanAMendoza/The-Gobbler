//
// COMP-GENG 421 - Tom Lupfer
//
// I2C module
//

#include "main.h"
#include "i2c.h"


#define I2C_DUTYCYCLE		LL_I2C_DUTYCYCLE_2
#define I2C_REQUEST_READ	0x01
#define I2C_REQUEST_WRITE	0x00
#define I2C_SPEEDCLOCK		400000


void I2cInit(void)
{
	LL_RCC_ClocksTypeDef RccClocks;

	// Enable the GPIOB port clock
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	// Configure SCL Pin as alt fcn, high speed, open drain, pull-up
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_8, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_8, LL_GPIO_AF_4);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_8, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_8, LL_GPIO_OUTPUT_OPENDRAIN);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_8, LL_GPIO_PULL_UP);

	// Configure SDA Pin as alt fcn, high speed, open drain, pull-up
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_8_15(GPIOB, LL_GPIO_PIN_9, LL_GPIO_AF_4);
	LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_9, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOB, LL_GPIO_PIN_9, LL_GPIO_OUTPUT_OPENDRAIN);
	LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_9, LL_GPIO_PULL_UP);

	// Enable the clock for I2C1
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

	// Configure SCL
	LL_I2C_Disable(I2C1);
	LL_RCC_GetSystemClocksFreq(&RccClocks);
	LL_I2C_ConfigSpeed(I2C1, RccClocks.PCLK1_Frequency, I2C_SPEEDCLOCK, I2C_DUTYCYCLE);
	LL_I2C_Enable(I2C1);
}


void I2cWrite(UInt8 DevAddr, UInt8 RegAddr, int NumBytes, UInt8 * DataBuf)
{
	// Prepare acknowledge for master data reception
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

	// Generate start condition
	LL_I2C_GenerateStartCondition(I2C1);

	// Wait for SB flag to be set
	while(!LL_I2C_IsActiveFlag_SB(I2C1))
		;

	// Write device address for a write
	LL_I2C_TransmitData8(I2C1, DevAddr | I2C_REQUEST_WRITE);

	// Wait for ADDR flag to be set, then clear it
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
		;
	LL_I2C_ClearFlag_ADDR(I2C1);

	// Write register address, wait for completion
	LL_I2C_TransmitData8(I2C1, RegAddr);
	while(!LL_I2C_IsActiveFlag_TXE(I2C1))
		;

	// Write data buffer
	while(NumBytes > 0)
	{
		// Write data byte, wait for completion
		LL_I2C_TransmitData8(I2C1, *DataBuf++);
		while(!LL_I2C_IsActiveFlag_TXE(I2C1))
			;
		NumBytes--;
	}

	// Generate stop condition
	LL_I2C_GenerateStopCondition(I2C1);
}


void I2cRead(UInt8 DevAddr, UInt8 RegAddr, int NumBytes, UInt8 * DataBuf)
{
	// Prepare ACK for master to receive data
	LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_ACK);

	// Generate start condition
	LL_I2C_GenerateStartCondition(I2C1);

	// Wait for SB flag to be set
	while(!LL_I2C_IsActiveFlag_SB(I2C1))
		;

	// Write device address for a write
	LL_I2C_TransmitData8(I2C1, DevAddr | I2C_REQUEST_WRITE);

	// Wait for ADDR flag to be set, then clear it
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
		;
	LL_I2C_ClearFlag_ADDR(I2C1);

	// Write register address, wait for completion
	LL_I2C_TransmitData8(I2C1, RegAddr);
	while(!LL_I2C_IsActiveFlag_TXE(I2C1))
		;

	// Generate repeated start condition
	LL_I2C_GenerateStartCondition(I2C1);

	// Wait for SB flag to be set
	while(!LL_I2C_IsActiveFlag_SB(I2C1))
		;

	// Write device address for a read
	LL_I2C_TransmitData8(I2C1, DevAddr | I2C_REQUEST_READ);

	// Wait for ADDR flag to be set, then clear it
	while(!LL_I2C_IsActiveFlag_ADDR(I2C1))
		;
	LL_I2C_ClearFlag_ADDR(I2C1);

	// Read data into buffer
	while(NumBytes > 0)
	{
		if (NumBytes == 1)
		{
			// Prepare NACK for master to receive last data byte
			LL_I2C_AcknowledgeNextData(I2C1, LL_I2C_NACK);
		}
		// Wait for read data, then read
		while(!LL_I2C_IsActiveFlag_RXNE(I2C1))
			;
		*DataBuf++ = LL_I2C_ReceiveData8(I2C1);

		NumBytes--;
	}

	// Generate stop condition
	LL_I2C_GenerateStopCondition(I2C1);
}
