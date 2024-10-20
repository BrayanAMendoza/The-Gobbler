//
// COMP-GENG 421 - Tom Lupfer
//
// RGB LED PWM module header file
//

#ifndef RGB_H_
#define RGB_H_

#define RGB_PWM_OFF			0
#define RGB_PWM_MAX			10


#define RGB_RED_DIS()		(LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4))
#define RGB_RED_ENA()		(LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4))
#define RGB_RED_TOG()		(LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_4))

#define RGB_GRN_DIS()		(LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_7))
#define RGB_GRN_ENA()		(LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7))
#define RGB_GRN_TOG()		(LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_7))

#define RGB_BLU_DIS()		(LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_9))
#define RGB_BLU_ENA()		(LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_9))
#define RGB_BLU_TOG()		(LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_9)


void	RgbInit(void);
void	RgbClock(void);
void	RgbRedPwmSet(int PwmSetting);
int		RgbRedPwmGet(void);
void	RgbGrnPwmSet(int PwmSetting);
int		RgbGrnPwmGet(void);
void	RgbBluPwmSet(int PwmSetting);
int		RgbBluPwmGet(void);

#endif	// RGB_H_
