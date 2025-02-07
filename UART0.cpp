/* Dependencies */
#include "UART.h"

/**
 * @brief Initializes the UART0 interface with register mappings.
 * 
 * @details This line creates an instance of the `__UART__` class named `UART0` and initializes it 
 *          with pointers to the relevant UART hardware registers for the ATmega328/328P microcontroller:
 *          - `UBRR0H` and `UBRR0L`: High and low bytes of the USART Baud Rate Register, 
 *            used to set the communication baud rate.
 *          - `UCSR0A`: USART Control and Status Register A, for monitoring the status of UART operations.
 *          - `UCSR0B`: USART Control and Status Register B, for enabling/disabling UART features 
 *            like RX, TX, and interrupts.
 *          - `UCSR0C`: USART Control and Status Register C, for configuring data frame formats 
 *            (parity, stop bits, data bits).
 *          - `UDR0`: USART Data Register, used for reading received data and writing data to transmit.
 * 
 * @note This approach allows the `__UART__` class to directly interact with the hardware registers,
 *       providing flexibility and abstraction for UART operations such as sending, receiving, and handling interrupts.
 * 
 * @see __UART__ Class Definition
 */
#if defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__)
__UART__ UART0 = __UART__(&UBRR0H, &UBRR0L, &UCSR0A, &UCSR0B, &UCSR0C, &UDR0);
#else
#error "Can't create instance of UART bus 0"
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
#if defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega328P__)
ISR(USART_RX_vect) { UART0.isrRX(); }
#elif defined(__AVR_ATmega328PB__)
ISR(USART0_RX_vect)  { UART0.isrRX(); }
#else
#error "Can't create an UART bus 0 RX interrupt routine"
#endif

/**
 * @brief USART Data Register Empty (UDRE) Interrupt Service Routine.
 * 
 * @details This ISR is triggered when the USART Data Register (UDR0) becomes empty, 
 *          indicating that the UART is ready to transmit the next byte of data. 
 *          It calls the `isrUDRE()` method of the `UART0` instance to handle 
 *          the transmission process.
 * 
 *          The `isrUDRE()` function checks if there is more data in the transmission 
 *          buffer (`txBuffer`). If data is available, it writes the next byte to `UDR0` 
 *          and updates the buffer pointers. If the buffer is empty, the ISR disables 
 *          the UDRE interrupt to prevent unnecessary triggering.
 * 
 * @note The UDRE interrupt allows non-blocking transmission, enabling the CPU to 
 *       perform other tasks while data is sent asynchronously.
 * 
 * @see __UART__::isrUDRE()
 */
#if defined(__AVR_ATmega328__) || \
    defined(__AVR_ATmega328P__)
ISR(USART_UDRE_vect) { UART0.isrUDRE(); }
#elif defined(__AVR_ATmega328PB__)
ISR(USART0_UDRE_vect)  { UART0.isrUDRE(); }
#else
#error "Can't create an UART bus 0 UDRE interrupt routine"
#endif