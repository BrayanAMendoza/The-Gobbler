//
// COMP-GENG 421 - Tom Lupfer
//
// Interrupt-driven full-duplex UART module
//

#include "main.h"
#include "uart.h"


#define	UART_RCV_BUFSIZE		1024			// max number of chars + 1 in UART rcv buf
#define	UART_XMT_BUFSIZE		1024			// max number of chars + 1 in UART xmt buf


static char		UartRcvBuf[UART_RCV_BUFSIZE];	// UART receive buffer
static int		UartRcvBufHeadIdx;				// UART receive buffer head index
static int		UartRcvBufTailIdx;				// UART receive buffer tail index

static char		UartXmtBuf[UART_XMT_BUFSIZE];	// UART transmit buffer
static int		UartXmtBufHeadIdx;				// UART receive buffer head index
static int		UartXmtBufTailIdx;				// UART receive buffer tail index


//
// Function to initialize the UART
//
void UartInit(void)
{
	// Reset the receive and transmit buffer head, tail indexes
	UartRcvBufHeadIdx = 0;
	UartRcvBufTailIdx = 0;
	UartXmtBufHeadIdx = 0;
	UartXmtBufTailIdx = 0;

	// (1) Enable GPIO clock and configure the USART2 pins

	// Enable the peripheral clock of GPIO Port
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);

	// Configure PA2 as USART2_TX: high speed, push pull, pull up, alternate function 7
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_7);

	// Configure PA3 as USART2_RX: high speed, alternate function 7
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_3, LL_GPIO_AF_7);

	// (2) NVIC Configuration for USART interrupts

	// Set priority for USARTx_IRQn
	// Enable USARTx_IRQn
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);

	// (3) Enable USART peripheral clock and clock source
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

	// (4) Configure USART functional parameters

	// Disable USART prior modifying configuration registers
	// Note: Commented as corresponding to Reset value
	// LL_USART_Disable(USARTx_INSTANCE);

	// TX/RX direction
	LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);

	// 8 data bit, 1 start bit, 1 stop bit, no parity
	LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

	// No Hardware Flow control
	// Reset value is LL_USART_HWCONTROL_NONE
	// LL_USART_SetHWFlowCtrl(USARTx_INSTANCE, LL_USART_HWCONTROL_NONE);

	// Oversampling by 16
	// Reset value is LL_USART_OVERSAMPLING_16
	// LL_USART_SetOverSampling(USARTx_INSTANCE, LL_USART_OVERSAMPLING_16);

	// Set Baudrate to 115200 using APB frequency set to 32000000 Hz
	// Frequency available for USART peripheral can also be calculated through LL RCC macro
	// Ex :
   	// 	   Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
   	// 	   In this example, Peripheral Clock is expected to be equal to 32000000 Hz => equal to SystemCoreClock
	LL_USART_SetBaudRate(USART2, SystemCoreClock, LL_USART_OVERSAMPLING_16, 115200);

	// (5) Enable USART
	LL_USART_Enable(USART2);

	// Enable RXNE interrupts
	LL_USART_EnableIT_RXNE(USART2);
}


//
// Function to determine if a received character is available
//
Bool UartCharAvail(void)
{
	// Return the receive buffer not empty status
	return (UartRcvBufHeadIdx != UartRcvBufTailIdx);
}


//
// Function to read a received character
//
char UartCharRead(void)
{
	char	RetChar;	// char to be returned

	// If the receive buffer is empty, return the null char
	if (UartRcvBufHeadIdx == UartRcvBufTailIdx)
	{
		return '\0';
	}

	// Read the value from the receive buffer, incr/wrap the head index
	RetChar = UartRcvBuf[UartRcvBufHeadIdx];
	if (++UartRcvBufHeadIdx == UART_RCV_BUFSIZE)
	{
		UartRcvBufHeadIdx = 0;
	}

	return RetChar;
}


//
// Function to write a character
//
void UartCharWrite(char OutChar)
{
	// Check that the transmit buffer is not full, otherwise throw away the char
	if ((!((UartXmtBufHeadIdx == 0) && (UartXmtBufTailIdx == (UART_XMT_BUFSIZE - 1)))) &&
		(!(UartXmtBufTailIdx == (UartXmtBufHeadIdx - 1))))
	{
		// Store the char in the transmit buffer, incr/wrap the tail index
		UartXmtBuf[UartXmtBufTailIdx] = OutChar;
		if (++UartXmtBufTailIdx == UART_XMT_BUFSIZE)
		{
			UartXmtBufTailIdx = 0;
		}
		// Enable the transmit data register empty interrupt
		  LL_USART_EnableIT_TXE(USART2);
	}
}


//
// Function to accommodate the C standard library printf()
//
void __io_putchar(char OutChar)
{
	UartCharWrite(OutChar);
}


//
// USART2 ISR
//
void USART2_IRQHandler(void)
{
	// Receive handler
	if (LL_USART_IsActiveFlag_RXNE(USART2))
	{
		// Read the char and store it at the current receive buffer tail index
		UartRcvBuf[UartRcvBufTailIdx] = LL_USART_ReceiveData8(USART2);

		// If the receive buffer is not full, incr/wrap the tail index
		if ((!((UartRcvBufHeadIdx == 0) && (UartRcvBufTailIdx == (UART_RCV_BUFSIZE - 1)))) &&
			(!(UartRcvBufTailIdx == (UartRcvBufHeadIdx - 1))))
		{
			if (++UartRcvBufTailIdx == UART_RCV_BUFSIZE)
			{
				UartRcvBufTailIdx = 0;
			}
		}

		// Otherwise, throw away the char (do nothing)
	}

	// Transmit handler
	if (LL_USART_IsActiveFlag_TXE(USART2))
	{
		// Check that the transmit buffer is not empty
		if (UartXmtBufHeadIdx != UartXmtBufTailIdx)
		{
			// Transmit the value from the buffer, incr/wrap the head index
			LL_USART_TransmitData8(USART2, UartXmtBuf[UartXmtBufHeadIdx]);
			if (++UartXmtBufHeadIdx == UART_XMT_BUFSIZE)
			{
				UartXmtBufHeadIdx = 0;
			}
		}
		// If the transmit buffer is empty, disable the transmit data register empty interrupt
		if (UartXmtBufHeadIdx == UartXmtBufTailIdx)
		{
			LL_USART_DisableIT_TXE(USART2);
		}
	}
}
