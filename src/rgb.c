//
// COMP-GENG 421 - Tom Lupfer
//
// RGB LED PWM module
//

#include "main.h"
#include "rgb.h"


#define RED_ST_OFF		0
#define RED_ST_ON		1

#define GRN_ST_OFF		0
#define GRN_ST_ON		1

#define BLU_ST_OFF		0
#define BLU_ST_ON		1


static int	RedPwmSetting = 0;
static int	GrnPwmSetting = 0;
static int	BluPwmSetting = 0;


void RgbInit(void)
{
	// Enable the clock for the PAx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// GPIO PA9 -> RGB blue LED (active low)
	// Initialize PA9 to an output, high (blue LED off)
	LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);


	// Enable the clock for the PBx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	// GPIO PB4 -> RGB red LED (active low)
	// Initialize PB4 to an output, high (red LED off)
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
	LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_4, LL_GPIO_MODE_OUTPUT);


	// Enable the clock for the PCx GPIO pins
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);

	// GPIO PC7 -> RGB green LED (active low)
	// Initialize PC7 to an output, high (green LED off)
	LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7);
	LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
}


void RgbClock(void)
{
	static int	RedState = RED_ST_OFF;
	static int	RedPwmCount = 0;
	static int	GrnState = GRN_ST_OFF;
	static int	GrnPwmCount = 0;
	static int	BluState = BLU_ST_OFF;
	static int	BluPwmCount = 0;

	RedPwmCount++;
	switch(RedState)
	{
	case RED_ST_OFF:
		if (RedPwmCount >= RGB_PWM_MAX)
		{
			RedPwmCount = 0;
			if (RedPwmSetting > 0)
			{
				RGB_RED_ENA();
				RedState = RED_ST_ON;
				break;
			}
		}
		break;

	case RED_ST_ON:
		if (RedPwmCount >= RedPwmSetting)
		{
			if (RedPwmSetting == RGB_PWM_MAX)
			{
				RedPwmCount = 0;
			}
			else
			{
				RGB_RED_DIS();
				RedState = RED_ST_OFF;
				break;
			}
		}
		break;
	}

	GrnPwmCount++;
	switch(GrnState)
	{
	case GRN_ST_OFF:
		if (GrnPwmCount >= RGB_PWM_MAX)
		{
			GrnPwmCount = 0;
			if (GrnPwmSetting > 0)
			{
				RGB_GRN_ENA();
				GrnState = GRN_ST_ON;
				break;
			}
		}
		break;

	case GRN_ST_ON:
		if (GrnPwmCount >= GrnPwmSetting)
		{
			if (GrnPwmSetting == RGB_PWM_MAX)
			{
				GrnPwmCount = 0;
			}
			else
			{
				RGB_GRN_DIS();
				GrnState = GRN_ST_OFF;
				break;
			}
		}
		break;
	}

	BluPwmCount++;
	switch(BluState)
	{
	case BLU_ST_OFF:
		if (BluPwmCount >= RGB_PWM_MAX)
		{
			BluPwmCount = 0;
			if (BluPwmSetting > 0)
			{
				RGB_BLU_ENA();
				BluState = BLU_ST_ON;
				break;
			}
		}
		break;

	case BLU_ST_ON:
		if (BluPwmCount >= BluPwmSetting)
		{
			if (BluPwmSetting == RGB_PWM_MAX)
			{
				BluPwmCount = 0;
			}
			else
			{
				RGB_BLU_DIS();
				BluState = BLU_ST_OFF;
				break;
			}
		}
		break;
	}
}


void RgbRedPwmSet(int PwmSetting)
{
	RedPwmSetting = (PwmSetting < 0)           ? 0           : PwmSetting;
	RedPwmSetting = (PwmSetting > RGB_PWM_MAX) ? RGB_PWM_MAX : RedPwmSetting;
}


int RgbRedPwmGet(void)
{
	return RedPwmSetting;
}


void RgbGrnPwmSet(int PwmSetting)
{
	GrnPwmSetting = (PwmSetting < 0)           ? 0           : PwmSetting;
	GrnPwmSetting = (PwmSetting > RGB_PWM_MAX) ? RGB_PWM_MAX : GrnPwmSetting;
}


int RgbGrnPwmGet(void)
{
	return GrnPwmSetting;
}


void RgbBluPwmSet(int PwmSetting)
{
	BluPwmSetting = (PwmSetting < 0)           ? 0           : PwmSetting;
	BluPwmSetting = (PwmSetting > RGB_PWM_MAX) ? RGB_PWM_MAX : BluPwmSetting;
}


int RgbBluPwmGet(void)
{
	return BluPwmSetting;
}
