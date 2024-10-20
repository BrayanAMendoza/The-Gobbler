//
// COMP-GENG 421 - Tom Lupfer
//
// Accelerometer module header file
//

#ifndef ACCEL_H_
#define ACCEL_H_


typedef struct _TiltState
{
	Bool Up;
	Bool Down;
	Bool Left;
	Bool Right;
} AccelTiltStateType;


void AccelInit(void);
void AccelTiltGet(AccelTiltStateType * TiltStatePtr);

//void AccelGenLut(void);
//void AccelClock(void);

#endif // ACCEL_H_
