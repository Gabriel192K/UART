#ifndef __UART_H__
#define __UART_H__

/* Dependencies */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/atomic.h>
#include "FlashStringHelper.h"

/**
 * @brief Size of the UART receive buffer.
 * @details This macro defines the size of the buffer used to store incoming UART data. The buffer is used to hold data received 
 *          over UART before it is processed. A size of 64 bytes is chosen, but this can be adjusted as needed.
 * @note The size is defined as a constant `uint8_t` value.
 */
#define UART_RX_BUFFER_SIZE (const uint8_t)64 /**< Size of the UART receive buffer */

/**
 * @brief Size of the UART transmit buffer.
 * @details This macro defines the size of the buffer used to store outgoing UART data before it is transmitted. The buffer
 *          holds data that needs to be sent over UART. A size of 64 bytes is chosen, but this can be adjusted based on application needs.
 * @note The size is defined as a constant `uint8_t` value.
 */
#define UART_TX_BUFFER_SIZE (const uint8_t)64 /**< Size of the UART transmit buffer */

/**
 * @brief UART class to control UART communication.
 */
class __UART__
{
    public:
        /**
         * @brief Constructor: Initializes the UART control, status, and data registers
         * @param ubrrh Pointer to the UART baud rate register high
         * @param ubrrl Pointer to the UART baud rate register low
         * @param ucsra Pointer to the UART control and status register A
         * @param ucsrb Pointer to the UART control and status register B
         * @param ucsrc Pointer to the UART control and status register C
         * @param udr   Pointer to the UART data register
         */
        __UART__(volatile uint8_t* ubrrh, \
                 volatile uint8_t* ubrrl, \
                 volatile uint8_t* ucsra, \
                 volatile uint8_t* ucsrb, \
                 volatile uint8_t* ucsrc, \
                 volatile uint8_t* udr);

        /**
         * @brief Destructor: Resets the register pointers to NULL
         */
        ~__UART__();

        /**
         * @brief Begins UART communication by setting the appropriate bits in the control registers
         * @param baudrate The baud rate to set
         * @return 1 if successful, 0 otherwise
         */
        const uint8_t begin(const uint32_t baudrate);

        /**
         * @brief Checks if data is available to read
         * @return 1 if data is available, 0 otherwise
         */
        const uint8_t available(void);

        /**
         * @brief Clears the receive buffer
         */
        void flush(void);

        /**
         * @brief Checks if UART is transmitting
         * @return 1 if transmitting, 0 otherwise
         */
        const uint8_t isTransmitting(void);

        /**
         * @brief Reads a single byte from the receive buffer
         * @return The byte read
         */
        const uint8_t read(void);

        /**
         * @brief Reads a byte array from the receive buffer
         * @param n Pointer to the byte array
         * @param size The size of the byte array
         */
        void read(uint8_t* n, const uint8_t size);

        /**
         * @brief Reads bytes from the receive buffer into a generic pointer
         * @param n Pointer to the destination memory location
         * @param size The number of bytes to read
         */
        void read(void* n, const uint8_t size);

        /**
         * @brief Writes a single byte to the transmit buffer
         * @param n The byte to write
         */
        void write(const uint8_t n);

        /**
         * @brief Writes a byte array to the transmit buffer
         * @param n Pointer to the byte array
         * @param size The size of the byte array
         */
        void write(const uint8_t* n, const uint8_t size);

        /**
         * @brief Writes bytes from a generic pointer to the transmit buffer
         * @param n Pointer to the source memory location
         * @param size The number of bytes to write
         */
        void write(const void* n, const uint8_t size);

        /**
         * @brief Prints a single character to the UART.
         * @param c Character to be transmitted.
         */
        void print(const char c);

        /**
         * @brief Prints a null-terminated string to the UART.
         * @param s Pointer to the null-terminated string to be transmitted.
         */
        void print(const char* s);

        /**
         * @brief Prints a string stored in program memory (Flash) to the UART.
         * @param s Reference to a FlashStringHelper object containing the string in program memory.
         */
        void print(const FlashStringHelper &s);

        /**
         * @brief Prints an unsigned 8-bit integer to the UART as ASCII digits.
         * @param n The uint8_t value to be printed (range 0-255).
         */
        void print(const uint8_t n);

        /**
         * @brief Prints an unsigned 16-bit integer to the UART as ASCII digits.
         * @param n The uint16_t value to be printed (range 0-65535).
         */
        void print(const uint16_t n);

        /**
         * @brief Prints an unsigned 32-bit integer to the UART as ASCII digits.
         * @param n The uint32_t value to be printed (range 0-4294967295).
         */
        void print(const uint32_t n);

        /**
         * @brief Prints a signed 8-bit integer to the UART as ASCII digits.
         * @param n The int8_t value to be printed (range -128 to 127).
         */
        void print(const int8_t n);

        /**
         * @brief Prints a signed 16-bit integer to the UART as ASCII digits.
         * @param n The int16_t value to be printed (range -32768 to 32767).
         */
        void print(const int16_t n);

        /**
         * @brief Prints a signed 32-bit integer to the UART as ASCII digits.
         * @param n The int32_t value to be printed (range -2147483648 to 2147483647).
         */
        void print(const int32_t n);

        /**
         * @brief Prints a newline character ('\n') to the UART.
         */
        void println(void);

        /**
         * @brief Prints a single character followed by a newline to the UART.
         * @param c The character to be printed.
         */
        void println(const char c);

        /**
         * @brief Prints a null-terminated string followed by a newline to the UART.
         * @param s Pointer to the null-terminated string to be transmitted.
         */
        void println(const char* s);

        /**
         * @brief Prints a string stored in program memory (Flash) followed by a newline to the UART.
         * @param s Reference to a FlashStringHelper object containing the string in program memory.
         */
        void println(const FlashStringHelper &s);

        /**
         * @brief Prints an unsigned 8-bit integer followed by a newline to the UART.
         * @param n The uint8_t value to be printed (range 0 to 255).
         */
        void println(const uint8_t n);

        /**
         * @brief Prints an unsigned 16-bit integer followed by a newline to the UART.
         * @param n The uint16_t value to be printed (range 0 to 65535).
         */
        void println(const uint16_t n);

        /**
         * @brief Prints an unsigned 32-bit integer followed by a newline to the UART.
         * @param n The uint32_t value to be printed (range 0 to 4294967295).
         */
        void println(const uint32_t n);

        /**
         * @brief Prints a signed 8-bit integer followed by a newline to the UART.
         * @param n The int8_t value to be printed (range -128 to 127).
         */
        void println(const int8_t n);

        /**
         * @brief Prints an unsigned 16-bit integer followed by a newline to the UART.
         * @param n The uint16_t value to be printed (range 0 to 65535).
         */
        void println(const int16_t n);

        /**
         * @brief Prints an unsigned 32-bit integer followed by a newline to the UART.
         * @param n The uint32_t value to be printed (range 0 to 4294967295).
         */
        void println(const int32_t n);

        /**
         * @brief Disables the UART communication and releases associated resources.
         * @return Returns 1 if UART was successfully disabled, 0 if UART was not started.
         */
        const uint8_t end(void);

        /**
         * @brief ISR (Interrupt Service Routine) for receiving data on the UART.
         */
        void isrRX(void);

        /**
         * @brief ISR (Interrupt Service Routine) for transmitting data on the UART.
         */
        void isrUDRE(void);

    private:
        /**
         * @brief Pointer to the UART Baud Rate Register High byte (UBRRH).
         * @details This pointer refers to the high byte of the UART baud rate register, which is used to set the baud rate for UART communication. 
         *          The baud rate is determined by both the high and low bytes of the UBRR register (UBRRH and UBRRL).
         *          This register is used in conjunction with UBRRL to configure the speed of serial communication.
         * @note The `volatile` keyword indicates that the value of this register can change unexpectedly (such as in hardware operations), 
         *       so the compiler must not optimize access to it.
         */
        volatile uint8_t* ubrrh; /**< Pointer to the high byte of the UART Baud Rate Register */

        /**
         * @brief Pointer to the UART Baud Rate Register Low byte (UBRRL).
         * @details This pointer refers to the low byte of the UART baud rate register, which is used together with the high byte (UBRRH) 
         *          to set the baud rate for UART communication. The baud rate is determined by the combined value of UBRRH and UBRRL.
         *          This register is critical for configuring the speed of the UART serial communication.
         * @note The `volatile` keyword indicates that the value of this register can be changed unexpectedly (e.g., by hardware or external events),
         *       so the compiler will not optimize access to it.
         */
        volatile uint8_t* ubrrl; /**< Pointer to the low byte of the UART Baud Rate Register */

        /**
         * @brief Pointer to the USART Control and Status Register A (UCSRA).
         * @details This pointer refers to the USART control and status register A, which contains various status flags related to UART communication,
         *          including flags for transmit and receive operations, framing errors, overrun errors, and more.
         *          This register is used to check the status of UART operations and manage various control settings.
         * @note The `volatile` keyword ensures that the compiler does not optimize the access to this register, 
         *       as the register can change at any time due to hardware actions (e.g., data received or transmitted).
         */
        volatile uint8_t* ucsra; /**< Pointer to the USART Control and Status Register A */

        /**
         * @brief Pointer to the USART Control and Status Register C (UCSRC).
         * @details This pointer refers to the USART Control and Status Register C, which is used to configure the frame format, such as
         *          data bits, parity bits, and stop bits, as well as enabling or disabling certain features (e.g., USART Mode, Asynchronous mode, etc.).
         * @note The `volatile` keyword ensures that the compiler does not optimize the access to this register, as it can change due to hardware behavior.
         */
        volatile uint8_t* ucsrc; /**< Pointer to the USART Control and Status Register C */

        /**
         * @brief Pointer to the UART Data Register (UDR).
         * @details This pointer refers to the UART Data Register, which holds the data to be transmitted or received. Writing to this register
         *          sends data over the UART, and reading from it retrieves received data.
         * @note The `volatile` keyword ensures that the compiler does not optimize access to this register, as data can change unexpectedly due to hardware actions.
         */
        volatile uint8_t* udr; /**< Pointer to the UART Data Register */

        /**
         * @brief Buffer for storing received UART data.
         * @details This buffer holds the incoming data received over UART. When data is received, it is placed in this buffer for further processing.
         *          The buffer size is defined by `UART_RX_BUFFER_SIZE`, and it uses circular indexing to efficiently handle incoming data.
         * @note The `volatile` keyword ensures that the compiler does not optimize the access to this buffer, as new data can be added at any time by the hardware.
         */
        volatile uint8_t rxBuffer[UART_RX_BUFFER_SIZE]; /**< Receive buffer for UART data */

        /**
         * @brief Buffer for storing data to be transmitted over UART.
         * @details This buffer holds the outgoing data to be sent over UART. The data is placed in this buffer before it is transmitted.
         *          The buffer size is defined by `UART_TX_BUFFER_SIZE`, and it uses circular indexing to manage data transmission.
         * @note The `volatile` keyword ensures that the compiler does not optimize the access to this buffer, as the data may change when written by the UART ISR.
         */
        volatile uint8_t txBuffer[UART_TX_BUFFER_SIZE]; /**< Transmit buffer for UART data */

        /**
         * @brief Indexes for managing the UART receive and transmit buffers.
         * @details These variables are used to keep track of the head and tail positions for the receive (`rxHead`, `rxTail`) and transmit (`txHead`, `txTail`) buffers.
         *          They are used to manage the circular buffer indexing, ensuring that data can be written and read efficiently.
         *          - `rxHead`: Points to the next position to store incoming data in `rxBuffer`.
         *          - `rxTail`: Points to the next position to read data from `rxBuffer`.
         *          - `txHead`: Points to the next position to send data from `txBuffer`.
         *          - `txTail`: Points to the next position to write data into `txBuffer`.
         * @note The `volatile` keyword ensures that these variables are not optimized by the compiler, as they can change during interrupt service routines.
         */
        volatile uint8_t rxHead, rxTail, txHead, txTail; /**< Indices for managing UART buffers */

        /**
         * @brief Flag indicating whether the UART has been initialized.
         * @details This variable tracks whether the UART communication has been started or not. It helps prevent performing UART operations before
         *          initialization, ensuring that the UART is properly configured before use.
         * @note This is a non-volatile variable since it is not expected to change in response to hardware actions.
         */
        uint8_t began; /**< Flag indicating whether the UART has been initialized */

};

#if defined(__AVR_ATmega328__)  || \
    defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__)
    extern __UART__ UART0;
#endif

#if defined(__AVR_ATmega328PB__)
    extern __UART__ UART1;
#endif



#endif