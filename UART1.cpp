/* Dependencies */
#include "UART.h"

#if defined(__AVR_ATmega328PB__)
__UART__ UART1 = __UART__(&UBRR1H, &UBRR1L, &UCSR1A, &UCSR1B, &UCSR1C, &UDR1);
#else
#error "Can't create instance of UART 1 bus"
#endif

/**
 * @brief USART Receive Complete Interrupt Service Routine (ISR).
 * 
 * @details This ISR is triggered when the USART has received a new byte of data.
 *          It calls the `isrRX()` method of the `UART0` object to handle the received data.
 *          The `isrRX()` function typically stores the received byte into the UART receive buffer.
 * 
 * @note This ISR ensures that incoming data is processed promptly to avoid buffer overruns.
 *       It operates asynchronously, triggered by the hardware when data is available in the USART data register.
 * 
 * @see __UART__::isrRX
 */
#if defined(__AVR_ATmega328PB__)
ISR(USART1_RX_vect)  { UART1.isrRX(); }
#else
#error "Can't create an UART bus 1 RX interrupt routine"
#endif

/**
 * @brief USART Data Register Empty (UDRE) Interrupt Service Routine.
 * 
 * @details This ISR is triggered when the USART Data Register (UDR0) becomes empty, 
 *          indicating that the UART is ready to transmit the next byte of data. 
 *          It calls the `isrUDRE()` method of the `UART1` instance to handle 
 *          the transmission process.
 * 
 *          The `isrUDRE()` function checks if there is more data in the transmission 
 *          buffer (`txBuffer`). If data is available, it writes the next byte to `UDR1` 
 *          and updates the buffer pointers. If the buffer is empty, the ISR disables 
 *          the UDRE interrupt to prevent unnecessary triggering.
 * 
 * @note The UDRE interrupt allows non-blocking transmission, enabling the CPU to 
 *       perform other tasks while data is sent asynchronously.
 * 
 * @see __UART__::isrUDRE()
 */
#if defined(__AVR_ATmega328PB__)
ISR(USART1_UDRE_vect)  { UART1.isrUDRE(); }
#else
#error "Can't create an UART bus 1 UDRE interrupt routine"
#endif