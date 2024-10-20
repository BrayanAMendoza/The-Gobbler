//
// COMP-GENG 421 - Tom Lupfer
//
// Debug module header file
//

#ifndef DBG_H_
#define DBG_H_

// PC4 -> Debug pin
#define DBG_CLR()	(LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_4))
#define DBG_SET()	(LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_4))
#define DBG_TOG()	(LL_GPIO_TogglePin(GPIOC, LL_GPIO_PIN_4)


void DbgInit(void);

#endif	// DBG_H_
