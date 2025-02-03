/* Dependencies */
#include "UART.h"

#if defined(__AVR_ATmega328PB__)
__UART__ UART1 = __UART__(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1);
#else
#error "Can't create instance of UART 1 bus"
#endif

/************************
Function: Interrupt Service Routine
Purpose:  Handling interrupts of UART RX
Input:    Interrupt vector
Return:   None
************************/
#if defined(__AVR_ATmega328PB__)
ISR(USART1_RX_vect)  { UART1.isrRX(); }
#else
#error "Can't create an UART bus 1 RX interrupt routine"
#endif

/************************
Function: Interrupt Service Routine
Purpose:  Handling interrupts of UART UDRE
Input:    Interrupt vector
Return:   None
************************/
#if defined(__AVR_ATmega328PB__)
ISR(USART1_UDRE_vect)  { UART1.isrUDRE(); }
#else
#error "Can't create an UART bus 1 UDRE interrupt routine"
#endif