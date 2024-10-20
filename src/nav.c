//
// COMP-GENG 421 - Tom Lupfer
//
// Navigation switch module
//

#include "main.h"
#include "nav.h"


#define DEBOUNCE_TICKS		50			// debounce delay (50 msec)

#define UP_ST_RELEASED		0			// up button - released
#define UP_ST_PRESSED		1			// up button - pressed
#define UP_ST_DEBOUNCE		2			// up button - debounce

#define UP_IS_PRESSED()		(LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_4))

#define DOWN_ST_RELEASED	0			// down button - released
#define DOWN_ST_PRESSED		1			// down button - pressed
#define DOWN_ST_DEBOUNCE	2			// down button - debounce

#define DOWN_IS_PRESSED()	(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_0))

#define LEFT_ST_RELEASED	0			// left button - released
#define LEFT_ST_PRESSED		1			// left button - pressed
#define LEFT_ST_DEBOUNCE	2			// left button - debounce

#define LEFT_IS_PRESSED()	(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_1))

#define RIGHT_ST_RELEASED	0			// right button - released
#define RIGHT_ST_PRESSED	1			// right button - pressed
#define RIGHT_ST_DEBOUNCE	2			// right button - debounce

#define RIGHT_IS_PRESSED()	(LL_GPIO_IsInputPinSet(GPIOC, LL_GPIO_PIN_0))

#define SELECT_ST_RELEASED	0			// select button - released
#define SELECT_ST_PRESSED	1			// select button - pressed
#define SELECT_ST_DEBOUNCE	2			// select button - debounce

#define SELECT_IS_PRESSED()	(LL_GPIO_IsInputPinSet(GPIOB, LL_GPIO_PIN_5))


static Bool	UpIsPressed = FALSE;		// up button pressed?
static Bool	DownIsPressed = FALSE;		// down button pressed?
static Bool	LeftIsPressed = FALSE;		// left button pressed?
static Bool	RightIsPressed = FALSE;		// right button pressed?
static Bool	SelectIsPressed = FALSE;	// select button pressed?


void NavInit(void)
{
	// Enable the clock for the PAx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// PA4 <- up button (active high)
	// Initialize PA4 as an input
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_INPUT);


	// Enable the clock for the PBx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	// PB0 <- down button (active high)
	// Initialize PB0 as an input
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);

	// PB5 <- select button (active high)
	// Initialize PB5 as an input
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_5, LL_GPIO_MODE_INPUT);


	// Enable the clock for the PCx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	// PC0 <- right button (active high)
	// Initialize PC0 as an input
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_0, LL_GPIO_MODE_INPUT);

	// PC1 <- left button (active high)
	// Initialize PC1 as an input
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_1, LL_GPIO_MODE_INPUT);
}


void NavClock(void)
{
	static int	UpState = UP_ST_RELEASED;
	static int	DownState = DOWN_ST_RELEASED;
	static int	LeftState = LEFT_ST_RELEASED;
	static int	RightState = RIGHT_ST_RELEASED;
	static int	SelectState = SELECT_ST_RELEASED;

	static int	UpDebounceTickCnt;			// up button debounce tick count
	static int	DownDebounceTickCnt;		// down button debounce tick count
	static int	LeftDebounceTickCnt;		// left button debounce tick count
	static int	RightDebounceTickCnt;		// right button debounce tick count
	static int	SelectDebounceTickCnt;		// select button debounce tick count

	switch (UpState)
	{
	case UP_ST_RELEASED:
		if (UP_IS_PRESSED())
		{
			UpDebounceTickCnt = DEBOUNCE_TICKS;
			UpState = UP_ST_DEBOUNCE;
		}
		break;

	case UP_ST_PRESSED:
		if (!UP_IS_PRESSED())
		{
			UpDebounceTickCnt = DEBOUNCE_TICKS;
			UpState = UP_ST_DEBOUNCE;
		}
		break;

	case UP_ST_DEBOUNCE:
		if (--UpDebounceTickCnt >= 0)
		{
			break;
		}
		if (UP_IS_PRESSED())
		{
			UpIsPressed = TRUE;
			UpState = UP_ST_PRESSED;
		}
		else
		{
			UpIsPressed = FALSE;
			UpState = UP_ST_RELEASED;
		}
		break;
	}

	switch (DownState)
	{
	case DOWN_ST_RELEASED:
		if (DOWN_IS_PRESSED())
		{
			DownDebounceTickCnt = DEBOUNCE_TICKS;
			DownState = DOWN_ST_DEBOUNCE;
		}
		break;

	case DOWN_ST_PRESSED:
		if (!DOWN_IS_PRESSED())
		{
			DownDebounceTickCnt = DEBOUNCE_TICKS;
			DownState = DOWN_ST_DEBOUNCE;
		}
		break;

	case DOWN_ST_DEBOUNCE:
		if (--DownDebounceTickCnt >= 0)
		{
			break;
		}
		if (DOWN_IS_PRESSED())
		{
			DownIsPressed = TRUE;
			DownState = DOWN_ST_PRESSED;
		}
		else
		{
			DownIsPressed = FALSE;
			DownState = DOWN_ST_RELEASED;
		}
		break;
	}

	switch (LeftState)
	{
	case LEFT_ST_RELEASED:
		if (LEFT_IS_PRESSED())
		{
			LeftDebounceTickCnt = DEBOUNCE_TICKS;
			LeftState = LEFT_ST_DEBOUNCE;
		}
		break;

	case LEFT_ST_PRESSED:
		if (!LEFT_IS_PRESSED())
		{
			LeftDebounceTickCnt = DEBOUNCE_TICKS;
			LeftState = LEFT_ST_DEBOUNCE;
		}
		break;

	case LEFT_ST_DEBOUNCE:
		if (--LeftDebounceTickCnt >= 0)
		{
			break;
		}
		if (LEFT_IS_PRESSED())
		{
			LeftIsPressed = TRUE;
			LeftState = LEFT_ST_PRESSED;
		}
		else
		{
			LeftIsPressed = FALSE;
			LeftState = LEFT_ST_RELEASED;
		}
		break;
	}

	switch (RightState)
	{
	case RIGHT_ST_RELEASED:
		if (RIGHT_IS_PRESSED())
		{
			RightDebounceTickCnt = DEBOUNCE_TICKS;
			RightState = RIGHT_ST_DEBOUNCE;
		}
		break;

	case RIGHT_ST_PRESSED:
		if (!RIGHT_IS_PRESSED())
		{
			RightDebounceTickCnt = DEBOUNCE_TICKS;
			RightState = RIGHT_ST_DEBOUNCE;
		}
		break;

	case RIGHT_ST_DEBOUNCE:
		if (--RightDebounceTickCnt >= 0)
		{
			break;
		}
		if (RIGHT_IS_PRESSED())
		{
			RightIsPressed = TRUE;
			RightState = RIGHT_ST_PRESSED;
		}
		else
		{
			RightIsPressed = FALSE;
			RightState = RIGHT_ST_RELEASED;
		}
		break;
	}

	switch (SelectState)
	{
	case SELECT_ST_RELEASED:
		if (SELECT_IS_PRESSED())
		{
			SelectDebounceTickCnt = DEBOUNCE_TICKS;
			SelectState = SELECT_ST_DEBOUNCE;
		}
		break;

	case SELECT_ST_PRESSED:
		if (!SELECT_IS_PRESSED())
		{
			SelectDebounceTickCnt = DEBOUNCE_TICKS;
			SelectState = SELECT_ST_DEBOUNCE;
		}
		break;

	case SELECT_ST_DEBOUNCE:
		if (--SelectDebounceTickCnt >= 0)
		{
			break;
		}
		if (SELECT_IS_PRESSED())
		{
			SelectIsPressed = TRUE;
			SelectState = SELECT_ST_PRESSED;
		}
		else
		{
			SelectIsPressed = FALSE;
			SelectState = SELECT_ST_RELEASED;
		}
		break;
	}
}


Bool NavUpIsPressed(void)
{
	return UpIsPressed;
}


void NavClearUpIsPressed(void)
{
	UpIsPressed = FALSE;
}


Bool NavDownIsPressed(void)
{
	return DownIsPressed;
}


void NavClearDownIsPressed(void)
{
	DownIsPressed = FALSE;
}


Bool NavLeftIsPressed(void)
{
	return LeftIsPressed;
}


void NavClearLeftIsPressed(void)
{
	LeftIsPressed = FALSE;
}


Bool NavRightIsPressed(void)
{
	return RightIsPressed;
}


void NavClearRightIsPressed(void)
{
	RightIsPressed = FALSE;
}


Bool NavSelectIsPressed(void)
{
	return SelectIsPressed;
}


void NavClearSelectIsPressed(void)
{
	SelectIsPressed = FALSE;
}
