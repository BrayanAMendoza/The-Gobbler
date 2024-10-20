//
// COMP-GENG 421 - Tom Lupfer
//
// Main header file
//

#ifndef MAIN_H_
#define MAIN_H_

#define USE_FULL_LL_DRIVER

#include "stm32l1xx.h"
#include "stm32l1xx_nucleo.h"
#include "stm32l1xx_ll_adc.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_dac.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_i2c.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_usart.h"
#include "stm32l1xx_ll_utils.h"


#define FALSE	0
#define TRUE	1


typedef int				Bool;	// Boolean
typedef signed int		Int;	// signed	integer
typedef unsigned int	UInt;	// unsigned	integer
typedef signed char		Int8;	// signed	 8-bit integer
typedef unsigned char	UInt8;	// unsigned	 8-bit integer
typedef signed short	Int16;	// signed	16-bit integer
typedef unsigned short	UInt16;	// unsigned	16-bit integer
typedef signed int		Int32;	// signed	32-bit integer
typedef unsigned int	UInt32;	// unsigned	32-bit integer

#endif	// MAIN_H_
