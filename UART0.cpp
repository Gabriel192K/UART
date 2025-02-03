/* Dependencies */
#include "UART.h"

#if defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__)
__UART__ UART0 = __UART__(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
#else
#error "Can't create instance of UART bus 0"
#endif

/************************
Function: Interrupt Service Routine
Purpose:  Handling interrupts of UART RX
Input:    Interrupt vector
Return:   None
************************/
#if defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega328P__)
ISR(USART_RX_vect) { UART0.isrRX(); }
#elif defined(__AVR_ATmega328PB__)
ISR(USART0_RX_vect)  { UART0.isrRX(); }
#else
#error "Can't create an UART bus 0 RX interrupt routine"
#endif

/************************
Function: Interrupt Service Routine
Purpose:  Handling interrupts of UART UDRE
Input:    Interrupt vector
Return:   None
************************/
#if defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega328P__)
ISR(USART_UDRE_vect) { UART0.isrUDRE(); }
#elif defined(__AVR_ATmega328PB__)
ISR(USART0_UDRE_vect)  { UART0.isrUDRE(); }
#else
#error "Can't create an UART bus 0 UDRE interrupt routine"
#endif