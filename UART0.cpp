/* Dependencies */
#include "UART.h"

#if defined(HAS_UART0)

__UART__ UART = __UART__(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);

/************************
Function: Interrupt Service Routine
Purpose:  Handling interrupts of UART RX
Input:    Interrupt vector
Return:   None
************************/
#if defined(__AVR_ATmega328P__)
ISR(USART_RX_vect)
#elif defined(__AVR_ATmega328PB__)
ISR(USART0_RX_vect)
#else
#error "Error! No UART RX vector defined"
#endif
{
    UART.isrRX();
}

/************************
Function: Interrupt Service Routine
Purpose:  Handling interrupts of UART UDRE
Input:    Interrupt vector
Return:   None
************************/
#if defined(__AVR_ATmega328P__)
ISR(USART_UDRE_vect)
#elif defined(__AVR_ATmega328PB__)
ISR(USART0_UDRE_vect)
#else
#error "Error! No UART UDRE vector defined"
#endif
{
    UART.isrUDRE();
}

#endif
