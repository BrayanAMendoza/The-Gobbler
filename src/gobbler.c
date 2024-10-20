//
// COMP-GENG 421 - Tom Lupfer
//
// Gobbler game module
//

#include "main.h"
#include "accel.h"
#include "lcd.h"
#include "nav.h"
#include "rgb.h"


#define GOBBLER_ST_INIT				0
#define GOBBLER_ST_WAIT_TO_START	1
#define GOBBLER_ST_START_RED		2
#define GOBBLER_ST_START_YEL		3
#define GOBBLER_ST_START_GRN		4
#define GOBBLER_ST_PLAY				5
#define GOBBLER_ST_TOO_SLOW			6
#define GOBBLER_ST_IN_TIME			7

#define RED_SLOW_REP_COUNT			3
#define RED_SLOW_OFF_TICKS			500
#define RED_SLOW_ON_TICKS			500

#define YEL_SLOW_REP_COUNT			1
#define YEL_SLOW_OFF_TICKS			500
#define YEL_SLOW_ON_TICKS			500

#define GRN_SLOW_REP_COUNT			1
#define GRN_SLOW_OFF_TICKS			500
#define GRN_SLOW_ON_TICKS			500

#define GRN_FAST_REP_COUNT			10
#define GRN_FAST_OFF_TICKS			250
#define GRN_FAST_ON_TICKS			250

#define RED_FAST_REP_COUNT			10
#define RED_FAST_OFF_TICKS			250
#define RED_FAST_ON_TICKS			250

#define TICKS_1SEC					1000
#define TIME_LIMIT_SECS				100

#define UPDATE_TICKS				100	// 100 msec

#define GOBBLER_POSX_MIN			0
#define GOBBLER_POSX_MAX			(LCD_PIXEL_WIDTH - LCD_GOBBLER_PIXEL_WIDTH)
#define GOBBLER_POSY_MIN			0
#define GOBBLER_POSY_MAX			(LCD_PIXEL_HEIGHT - LCD_GOBBLER_PIXEL_HEIGHT)

#define GOBBLER_POSX_CENTER			((LCD_PIXEL_WIDTH / 2) - (LCD_GOBBLER_PIXEL_WIDTH / 2))
#define GOBBLER_POSY_CENTER			((LCD_PIXEL_HEIGHT / 2) - (LCD_GOBBLER_PIXEL_HEIGHT / 2))


static Int ElapsedTickCount;
static Int ElapsedSecondsCount;
static Bool ElapsedSecondsUpdateFlag;
static Int GobblerCurPosX;
static Int GobblerCurPosY;


static void RenderInitialPlayScreen(void);
static void GobblerUpdate(void);
static void ElapsedTimeUpdate(void);


void GobblerClock(void)
{
	static Int GobblerState = GOBBLER_ST_INIT;
	static Int DelayCount;
	static Int RepCount;

	switch (GobblerState)
	{
	case GOBBLER_ST_INIT:
		RgbRedPwmSet(RGB_PWM_OFF);
		RgbGrnPwmSet(RGB_PWM_OFF);
		RgbBluPwmSet(RGB_PWM_OFF);
		LcdClear();
		LcdPrintf(0, 5, LCD_CHAR_NORMAL, "THE GOBBLER");
		LcdPrintf(2, 0, LCD_CHAR_NORMAL, "Press Select To Start");
		NavClearSelectIsPressed();
		GobblerState = GOBBLER_ST_WAIT_TO_START;
		break;

	case GOBBLER_ST_WAIT_TO_START:
		if(NavSelectIsPressed())
		{
			LcdClear();
			DelayCount = RED_SLOW_OFF_TICKS;
			RepCount = RED_SLOW_REP_COUNT;
			GobblerState = GOBBLER_ST_START_RED;
			break;
		}
		break;

	case GOBBLER_ST_START_RED:
		if (--DelayCount > 0)
		{
			break;
		}
		if (RgbRedPwmGet() == RGB_PWM_OFF)
		{
			RgbRedPwmSet(RGB_PWM_MAX);
			DelayCount = RED_SLOW_ON_TICKS;
			break;
		}
		RgbRedPwmSet(RGB_PWM_OFF);
		if (--RepCount <= 0)
		{
			DelayCount = YEL_SLOW_OFF_TICKS;
			RepCount = YEL_SLOW_REP_COUNT;
			GobblerState = GOBBLER_ST_START_YEL;
			break;
		}
		DelayCount = RED_SLOW_OFF_TICKS;
		break;

	case GOBBLER_ST_START_YEL:
		if (--DelayCount > 0)
		{
			break;
		}
		if (RgbRedPwmGet() == RGB_PWM_OFF)
		{
			RgbRedPwmSet(RGB_PWM_MAX);
			RgbGrnPwmSet(RGB_PWM_MAX);
			DelayCount = YEL_SLOW_ON_TICKS;
			break;
		}
		RgbRedPwmSet(RGB_PWM_OFF);
		RgbGrnPwmSet(RGB_PWM_OFF);
		if (--RepCount <= 0)
		{
			DelayCount = GRN_SLOW_OFF_TICKS;
			RepCount = GRN_SLOW_REP_COUNT;
			GobblerState = GOBBLER_ST_START_GRN;
			break;
		}
		DelayCount = YEL_SLOW_OFF_TICKS;
		break;

	case GOBBLER_ST_START_GRN:
		if (--DelayCount > 0)
		{
			break;
		}
		if (RgbGrnPwmGet() == RGB_PWM_OFF)
		{
			RgbGrnPwmSet(RGB_PWM_MAX);
			DelayCount = GRN_SLOW_ON_TICKS;
			break;
		}
		RgbGrnPwmSet(RGB_PWM_OFF);
		if (--RepCount <= 0)
		{
			DelayCount = UPDATE_TICKS;
			ElapsedTickCount = TICKS_1SEC;
			ElapsedSecondsCount = 0;
			ElapsedSecondsUpdateFlag = TRUE;
			RenderInitialPlayScreen();
			NavClearSelectIsPressed();
			GobblerState = GOBBLER_ST_PLAY;
			break;
		}
		DelayCount = GRN_SLOW_OFF_TICKS;
		break;

	case GOBBLER_ST_PLAY:
		if(NavSelectIsPressed())
		{
			LcdClear();
			LcdPrintf(0, 6, LCD_CHAR_NORMAL, "You Win!!");
			LcdPrintf(2, 2, LCD_CHAR_NORMAL, "Time: %02d Seconds", ElapsedSecondsCount);
			DelayCount = GRN_FAST_OFF_TICKS;
			RepCount = GRN_FAST_REP_COUNT;
			GobblerState = GOBBLER_ST_IN_TIME;
			break;
		}

		if (--ElapsedTickCount <= 0)
		{
			ElapsedTickCount = TICKS_1SEC;
			ElapsedSecondsCount++;
			ElapsedSecondsUpdateFlag = TRUE;
		}

		if (--DelayCount > 0)
		{
			break;
		}
		DelayCount = UPDATE_TICKS;

		ElapsedTimeUpdate();
		GobblerUpdate();

		if (ElapsedSecondsCount >= TIME_LIMIT_SECS)
		{
			LcdClear();
			LcdPrintf(0, 7, LCD_CHAR_NORMAL, "Loser!!");
			LcdPrintf(2, 0, LCD_CHAR_NORMAL, "You need to speed up!");
			DelayCount = RED_FAST_OFF_TICKS;
			RepCount = RED_FAST_REP_COUNT;
			GobblerState = GOBBLER_ST_TOO_SLOW;
			break;
		}
		break;

	case GOBBLER_ST_TOO_SLOW:
		if (--DelayCount > 0)
		{
			break;
		}
		if (RgbRedPwmGet() == RGB_PWM_OFF)
		{
			RgbRedPwmSet(RGB_PWM_MAX);
			DelayCount = RED_FAST_ON_TICKS;
			break;
		}
		RgbRedPwmSet(RGB_PWM_OFF);
		if (--RepCount <= 0)
		{
			GobblerState = GOBBLER_ST_INIT;
			break;
		}
		DelayCount = RED_FAST_OFF_TICKS;
		break;

	case GOBBLER_ST_IN_TIME:
		if (--DelayCount > 0)
		{
			break;
		}
		if (RgbGrnPwmGet() == RGB_PWM_OFF)
		{
			RgbGrnPwmSet(RGB_PWM_MAX);
			DelayCount = GRN_FAST_ON_TICKS;
			break;
		}
		RgbGrnPwmSet(RGB_PWM_OFF);
		if (--RepCount <= 0)
		{
			GobblerState = GOBBLER_ST_INIT;
			break;
		}
		DelayCount = GRN_FAST_OFF_TICKS;
		break;
	}
}


static void RenderInitialPlayScreen(void)
{
	GobblerCurPosX = GOBBLER_POSX_CENTER;
	GobblerCurPosY = GOBBLER_POSY_CENTER;
	LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY, LCD_OBJ_RENDER);
	LcdRenderObject(LCD_OBJ_GOBLET, 100, 4, LCD_OBJ_RENDER);
}


static void GobblerUpdate(void)
{
	AccelTiltStateType TiltState;

	AccelTiltGet(&TiltState);

	LcdPrintf(0, 0, LCD_CHAR_NORMAL, "%s", TiltState.Up    ? "Up"    : "  ");
	LcdPrintf(1, 0, LCD_CHAR_NORMAL, "%s", TiltState.Down  ? "Down"  : "    ");
	LcdPrintf(2, 0, LCD_CHAR_NORMAL, "%s", TiltState.Left  ? "Left"  : "    ");
	LcdPrintf(3, 0, LCD_CHAR_NORMAL, "%s", TiltState.Right ? "Right" : "     ");


	if (TiltState.Up)
	{
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY--, LCD_OBJ_ERASE);
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY, LCD_OBJ_RENDER);
	}
	if (TiltState.Down)
	{
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY++, LCD_OBJ_ERASE);
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY, LCD_OBJ_RENDER);
	}
	if (TiltState.Right)
	{
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX++, GobblerCurPosY, LCD_OBJ_ERASE);
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY, LCD_OBJ_RENDER);
	}
	if (TiltState.Left)
	{
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX--, GobblerCurPosY, LCD_OBJ_ERASE);
		LcdRenderObject(LCD_OBJ_GOBBLER, GobblerCurPosX, GobblerCurPosY, LCD_OBJ_RENDER);
	}
}


static void ElapsedTimeUpdate(void)
{
	if (ElapsedSecondsUpdateFlag)
	{
		LcdPrintf(0, 19, LCD_CHAR_NORMAL, "%02d", ElapsedSecondsCount);
		ElapsedSecondsUpdateFlag = FALSE;
	}
}
