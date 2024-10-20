//
// COMP-GENG 421 - Tom Lupfer
//
// Accelerometer module
//

#include <math.h>

#include "main.h"
#include "accel.h"
#include "eprintf.h"
#include "i2c.h"


#define ALERT_BITMASK		0x40	// Alert bit in XOUT, YOUT, ZOUT regs

#define PI					3.14159

#define I2C_ADDR_ACCEL		0x98

#define RDG_BITS			6
#define RDG_SHIFT			(32 - RDG_BITS)

#define RDG_1G				22

#define RDG_IDX_XOUT		0
#define RDG_IDX_YOUT		1
#define RDG_IDX_ZOUT		2

#define REG_ADDR_XOUT		0x00
#define REG_ADDR_YOUT		0x01
#define REG_ADDR_ZOUT		0x02
#define REG_ADDR_INTSU		0x06
#define REG_ADDR_MODE		0x07
#define REG_ADDR_SR			0x08
#define REG_ADDR_PDET		0x09
#define REG_ADDR_PD			0x0A

#define TILT_TH				15
#define TILT_UP_X_LT		(90 - TILT_TH)
#define TILT_DOWN_X_GT		(90 + TILT_TH)
#define TILT_LEFT_Y_LT		(90 - TILT_TH)
#define TILT_RIGHT_Y_GT		(90 + TILT_TH)

#define XYZ_BUF_SIZE		3


// Arccosine lookup table
const static Int ArcCosineArray[45] =
{
	180, 162, 155, 149, 144,
	140, 136, 132, 129, 126,
	123, 120, 117, 114, 111,
	108, 105, 103, 100,  97,
	 95,  92,  90,  87,  84,
	 82,  79,  76,  74,  71,
	 68,  65,  62,  60,  56,
	 53,  50,  47,  43,  39,
	 35,  30,  24,  17,   0,
};


static Int Rdg2Deg(UInt8 Reading);


void AccelInit(void)
{
	UInt8 XmtBuf[1];

	// Clear all of the control registers to their default values
	XmtBuf[0] = 0x00;
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_MODE,  1, XmtBuf);	// clear TON bit to get out of test mode
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_INTSU, 1, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_SR,    1, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_PDET,  1, XmtBuf);
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_PD,    1, XmtBuf);

	// Enable the active mode
	XmtBuf[0] = 0x01;
	I2cWrite(I2C_ADDR_ACCEL, REG_ADDR_MODE,  1, XmtBuf);
}


void AccelTiltGet(AccelTiltStateType * TiltStatePtr)
{
	UInt8 RdgBuf[XYZ_BUF_SIZE];

	// Read the X, Y, Z values until there are no alert bits set
	do
	{
		I2cRead(I2C_ADDR_ACCEL, REG_ADDR_XOUT, XYZ_BUF_SIZE, RdgBuf);
	}
	while ((RdgBuf[RDG_IDX_XOUT] | RdgBuf[RDG_IDX_YOUT] | RdgBuf[RDG_IDX_ZOUT]) & ALERT_BITMASK);

	// Determine the tilt states
	TiltStatePtr->Up = Rdg2Deg(RdgBuf[RDG_IDX_XOUT]) < TILT_UP_X_LT ? TRUE : FALSE;
	TiltStatePtr->Down = Rdg2Deg(RdgBuf[RDG_IDX_XOUT]) > TILT_DOWN_X_GT ? TRUE : FALSE;
	TiltStatePtr->Left = Rdg2Deg(RdgBuf[RDG_IDX_YOUT]) < TILT_LEFT_Y_LT ? TRUE : FALSE;
	TiltStatePtr->Right = Rdg2Deg(RdgBuf[RDG_IDX_YOUT]) > TILT_RIGHT_Y_GT ? TRUE : FALSE;
}


static Int Rdg2Deg(UInt8 Reading)
{
	Int SignedReading;

	// Convert the 6-bit signed reading to a signed integer
	SignedReading = (Int) ((Reading << RDG_SHIFT) >> RDG_SHIFT);

	// Constrain the reading to +/- 1g
	SignedReading = SignedReading < -RDG_1G ? -RDG_1G : SignedReading;
	SignedReading = SignedReading >  RDG_1G ?  RDG_1G : SignedReading;

	// Return the arccosine via the lookup table
	return ArcCosineArray[SignedReading + RDG_1G];
}


#ifdef ARCHIVE

#define ARCCOS_LUT_SIZE		(2 * RDG_1G + 1)


// Function to generate the arccosine lookup table
void AccelGenLut(void)
{
	Int SignedReading;

	eprintf("const static Int ArcCosineArray[%d] = \n", ARCCOS_LUT_SIZE);
	eprintf("{\n");
	for (SignedReading = -RDG_1G; SignedReading <= RDG_1G; )
	{
		eprintf("\t%3d, ",  (UInt) (180 * acos((float) SignedReading++ / RDG_1G) / PI));
		eprintf("%3d, ",    (UInt) (180 * acos((float) SignedReading++ / RDG_1G) / PI));
		eprintf("%3d, ",    (UInt) (180 * acos((float) SignedReading++ / RDG_1G) / PI));
		eprintf("%3d, ",    (UInt) (180 * acos((float) SignedReading++ / RDG_1G) / PI));
		eprintf("%3d,\n",   (UInt) (180 * acos((float) SignedReading++ / RDG_1G) / PI));
	}
	eprintf("};\n");
}


#define REFRESH_TICKS		100


void AccelClock(void)
{
	static Int TickCount = REFRESH_TICKS;

	UInt8 RdgBuf[XYZ_BUF_SIZE];

	// Read the accelerometer and display the results periodically
	if (--TickCount > 0)
	{
		return;
	}
	TickCount = REFRESH_TICKS;

	// Read the X, Y, Z values until there are no alert bits set
	do
	{
		I2cRead(I2C_ADDR_ACCEL, REG_ADDR_XOUT, XYZ_BUF_SIZE, RdgBuf);
	}
	while ((RdgBuf[RDG_IDX_XOUT] | RdgBuf[RDG_IDX_YOUT] | RdgBuf[RDG_IDX_ZOUT]) & ALERT_BITMASK);

	eprintf("X: %3d  Y: %3d  Z: %3d\n",
			Rdg2Deg(RdgBuf[RDG_IDX_XOUT]),
			Rdg2Deg(RdgBuf[RDG_IDX_YOUT]),
			Rdg2Deg(RdgBuf[RDG_IDX_ZOUT]));
}
#endif	// ARCHIVE
