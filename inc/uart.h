//
// COMP-GENG 421 - Tom Lupfer
//
// Interrupt-driven full-duplex UART module header file
//

#ifndef UART_H_
#define UART_H_

void UartInit(void);
Bool UartCharAvail(void);
char UartCharRead(void);
void UartCharWrite(char OutChar);
void USART2_IRQHandler(void);

#endif	// UART_H_
