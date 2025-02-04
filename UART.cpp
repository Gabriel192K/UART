/* Dependencies */
#include "UART.h"

/**
 * @brief Constructor: Initializes the UART control, status, and data registers
 * @param ubrrh Pointer to the UART baud rate register high
 * @param ubrrl Pointer to the UART baud rate register low
 * @param ucsra Pointer to the UART control and status register A
 * @param ucsrb Pointer to the UART control and status register B
 * @param ucsrc Pointer to the UART control and status register C
 * @param udr   Pointer to the UART data register
 */
__UART__::__UART__(volatile uint8_t* ubrrh, \
                   volatile uint8_t* ubrrl, \
                   volatile uint8_t* ucsra, \
                   volatile uint8_t* ucsrb, \
                   volatile uint8_t* ucsrc, \
                   volatile uint8_t* udr)
{
    this->ubrrh = ubrrh;
    this->ubrrl = ubrrl;
    this->ucsra = ucsra;
    this->ucsrb = ucsrb;
    this->ucsrc = ucsrc;
    this->udr = udr;
}

/**
 * @brief Destructor: Resets the register pointers to NULL
 */
__UART__::~__UART__()
{
    this->ubrrh = NULL;
    this->ubrrl = NULL;
    this->ucsra = NULL;
    this->ucsrb = NULL;
    this->ucsrc = NULL;
    this->udr = NULL;
}

/**
 * @brief Begins UART communication by setting the appropriate bits in the control registers
 * @param baudrate The baud rate to set
 * @return 1 if successful, 0 otherwise
 */
const uint8_t __UART__::begin(const uint32_t baudrate)
{
    if (this->began)
        return (0);

    this->began = 1;

    sei(); /*!< Enable global interrupts */

    uint16_t prescale = (F_CPU / 4 / baudrate - 1) / 2; /*!< Calculate the prescale value */
    
    #if defined(__AVR_ATmega328__) || \
        defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    if (prescale > 0x0FFF)
    {
        prescale = (F_CPU / 8 / baudrate - 1) / 2;
        *this->ucsra = 0;
    } 
    else
        *this->ucsra |= (1 << U2X0);
    
    *this->ubrrh = (uint8_t)(prescale >> 8); /*!< Write <LSB> of the prescale */
    *this->ubrrl = (uint8_t)prescale;        /*!< Write <MSB> of the prescale */
    *this->ucsrc |= (1 << UCSZ01) | \
                    (1 << UCSZ00);           /*!< Set the data frame format to 8-bit */
    *this->ucsrb |= (1 << RXEN0) | \
                    (1 << RXCIE0) | \
                    (1 << TXEN0);            /*!< Enable RX, RX ISR, TX */
    #else
        #error "Can not config UART bus in begin()"
    #endif
    return (1);
}

/**
 * @brief Checks if data is available to read
 * @return 1 if data is available, 0 otherwise
 */
const uint8_t __UART__::available(void)
{
    uint8_t bytes = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        bytes = (UART_RX_BUFFER_SIZE + this->rxHead - this->rxTail) % UART_RX_BUFFER_SIZE;
    return (bytes);
}

/**
 * @brief Clears the receive buffer
 */
void __UART__::flush(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        this->rxHead = this->rxTail;
}

/**
 * @brief Checks if UART is transmitting
 * @return 1 if transmitting, 0 otherwise
 */
const uint8_t __UART__::isTransmitting(void)
{
    #if defined(__AVR_ATmega328__) || \
        defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    return ((*this->ucsrb & (1 << UDRIE0)) != 0);
    #else
    #error "Can't check if isTransmitting() the UART bus"
    #endif
}

/**
 * @brief Reads a single byte from the receive buffer
 * @return The byte read
 */
const uint8_t __UART__::read(void)
{
    if (this->rxHead == this->rxTail)
        return (0);
	
	uint8_t byte = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        byte = this->rxBuffer[this->rxTail];
        this->rxTail = (this->rxTail + 1) % UART_RX_BUFFER_SIZE;
    }
    return (byte);
}

/**
 * @brief Reads a byte array from the receive buffer
 * @param n Pointer to the byte array
 * @param size The size of the byte array
 */
void __UART__::read(uint8_t* n, const uint8_t size)
{
    for (uint8_t i = 0; i < size;)
        if (this->available())
            n[i++] = this->read();
}

/**
 * @brief Reads bytes from the receive buffer into a generic pointer
 * @param n Pointer to the destination memory location
 * @param size The number of bytes to read
 */
void __UART__::read(void* n, const uint8_t size)
{
    this->read((uint8_t*)n, size);
}

/**
 * @brief Writes a single byte to the transmit buffer
 * @param n The byte to write
 */
void __UART__::write(const uint8_t n)
{
    const uint8_t head = (this->txHead + 1) % UART_TX_BUFFER_SIZE;
    while (head == this->txTail);

    this->txBuffer[this->txHead] = n;
    this->txHead = head;

    #if defined(__AVR_ATmega328__) || \
        defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    *this->ucsrb = *this->ucsrb | (1 << UDRIE0);
    #else
    #error "Can't turn on the UDRIE interrupt"
    #endif
}

/**
 * @brief Writes a byte array to the transmit buffer
 * @param n Pointer to the byte array
 * @param size The size of the byte array
 */
void __UART__::write(const uint8_t* n, const uint8_t size)
{
    for (const uint8_t* p = n; p < (n + size); p++)
        this->write(*p);
}

/**
 * @brief Writes bytes from a generic pointer to the transmit buffer
 * @param n Pointer to the source memory location
 * @param size The number of bytes to write
 */
void __UART__::write(const void* n, const uint8_t size)
{
    this->write((const uint8_t*)n, size);
}

/**
 * @brief Prints a single character to the UART.
 * @param c Character to be transmitted.
 * @details Converts the char to uint8_t and writes it to the UART using write().
 */
void __UART__::print(const char c)
{
    this->write((const uint8_t)c);
}

/**
 * @brief Prints a null-terminated string to the UART.
 * @param s Pointer to the null-terminated string to be transmitted.
 * @details Iterates through each character of the string until the null terminator is reached,
 *          converting each char to uint8_t and writing it to the UART using write().
 */
void __UART__::print(const char* s)
{
    while (*s)
        this->write((const uint8_t)*s++);
}

/**
 * @brief Prints a string stored in program memory (Flash) to the UART.
 * @param s Reference to a FlashStringHelper object containing the string in program memory.
 * @details Reads the string byte by byte from program memory using pgm_read_byte(),
 *          iterating until the null terminator is reached. Each byte is converted
 *          to uint8_t and written to the UART using write().
 * @note This method is specific for AVR microcontrollers where strings can be stored
 *       in program memory (Flash) to save RAM.
 */
void __UART__::print(const FlashStringHelper &s)
{
    const char* ptr = s.get();
    while (pgm_read_byte(ptr))
        this->write((const uint8_t)pgm_read_byte(ptr++));
}

/**
 * @brief Prints an unsigned 8-bit integer to the UART as ASCII digits.
 * @param n The uint8_t value to be printed (range 0-255).
 * @details Converts the number to ASCII digits by:
 *          - First printing hundreds digit if n > 99
 *          - Then printing tens digit if n > 9
 *          - Always printing ones digit
 *          Each digit is converted to ASCII by adding '0' offset.
 * @note Does not print leading zeros.
 */
void __UART__::print(const uint8_t n)
{
    if (n > 99) this->print((const char)(((n / 100) % 10) + '0'));
    if (n > 9)  this->print((const char)(((n / 10) % 10) + '0'));
    this->print((const char)((n % 10) + '0'));
}

/**
 * @brief Prints an unsigned 16-bit integer to the UART as ASCII digits.
 * @param n The uint16_t value to be printed (range 0-65535).
 * @details Converts the number to ASCII digits by:
 *          - First printing ten thousands digit if n > 9999
 *          - Then thousands digit if n > 999
 *          - Then hundreds digit if n > 99
 *          - Then tens digit if n > 9
 *          - Always printing ones digit
 *          Each digit is converted to ASCII by adding '0' offset.
 * @note Does not print leading zeros.
 */
void __UART__::print(const uint16_t n)
{
    if (n > 9999) this->print((const char)(((n / 10000) % 10) + '0'));
    if (n > 999)  this->print((const char)(((n / 1000) % 10) + '0'));
    if (n > 99)   this->print((const char)(((n / 100) % 10) + '0'));
    if (n > 9)    this->print((const char)(((n / 10) % 10) + '0'));
    this->print((const char)((n % 10) + '0'));
}

/**
 * @brief Prints an unsigned 32-bit integer to the UART as ASCII digits.
 * @param n The uint32_t value to be printed (range 0-4294967295).
 * @details Converts the number to ASCII digits by sequentially checking and printing:
 *          - Billions digit if n > 999999999
 *          - Hundred millions digit if n > 99999999
 *          - Ten millions digit if n > 9999999
 *          - Millions digit if n > 999999
 *          - Hundred thousands digit if n > 99999
 *          - Ten thousands digit if n > 9999
 *          - Thousands digit if n > 999
 *          - Hundreds digit if n > 99
 *          - Tens digit if n > 9
 *          - Always printing ones digit
 *          Each digit is converted to ASCII by adding '0' offset.
 * @note Does not print leading zeros.
 */
void __UART__::print(const uint32_t n)
{
    if (n > 999999999) this->print((const char)(((n / 1000000000) % 10) + '0'));
    if (n > 99999999)  this->print((const char)(((n / 100000000) % 10) + '0'));
    if (n > 9999999)   this->print((const char)(((n / 10000000) % 10) + '0'));
    if (n > 999999)    this->print((const char)(((n / 1000000) % 10) + '0'));
    if (n > 99999)     this->print((const char)(((n / 100000) % 10) + '0'));
    if (n > 9999)      this->print((const char)(((n / 10000) % 10) + '0'));
    if (n > 999)       this->print((const char)(((n / 1000) % 10) + '0'));
    if (n > 99)        this->print((const char)(((n / 100) % 10) + '0'));
    if (n > 9)         this->print((const char)(((n / 10) % 10) + '0'));
    this->print((const char)((n % 10) + '0'));
}

/**
 * @brief Prints a signed 8-bit integer to the UART as ASCII digits.
 * @param n The int8_t value to be printed (range -128 to 127).
 * @details If the number is negative:
 *          - First prints a minus sign
 *          - Converts to positive by negation
 *          - Calls print(uint8_t) to handle the digit conversion
 *          If positive, directly calls print(uint8_t).
 * @note Uses print(uint8_t) internally for the actual digit conversion.
 */
void __UART__::print(const int8_t n)
{
    if (n < 0)
    {
        this->write((const uint8_t)'-');
        this->print((const uint8_t)-n);
    }
    else
        this->print((const uint8_t)n);
}

/**
* @brief Prints a signed 16-bit integer to the UART as ASCII digits.
* @param n The int16_t value to be printed (range -32768 to 32767).
* @details If the number is negative:
*          - First prints a minus sign
*          - Converts to positive by negation
*          - Calls print(uint16_t) to handle the digit conversion
*          If positive, directly calls print(uint16_t).
* @note Uses print(uint16_t) internally for the actual digit conversion.
*/
void __UART__::print(const int16_t n)
{
    if (n < 0)
    {
        this->write((const uint8_t)'-');
        this->print((const uint16_t)-n);
    }
    else
        this->print((const uint16_t)n);
}

/**
 * @brief Prints a signed 32-bit integer to the UART as ASCII digits.
 * @param n The int32_t value to be printed (range -2,147,483,648 to 2,147,483,647).
 * @details If the number is negative:
 *          - First prints a minus sign ('-') using write().
 *          - Converts the number to positive by negation (handling the edge case of INT32_MIN).
 *          - Calls print(uint32_t) to handle the digit conversion.
 *          If positive, directly calls print(uint32_t).
 * @note Uses print(uint32_t) internally for the actual digit conversion.
 */
void __UART__::print(const int32_t n)
{
    if (n < 0)
    {
        this->write((const uint8_t)'-');
        this->print((const uint32_t)-n);
    }
    else
        this->print((const uint32_t)n);
}

/**
 * @brief Prints a newline character to the UART.
 * @details Sends the newline character ('\n') to the UART using write().
 *          This moves the cursor to the beginning of the next line 
 *          in terminal displays or serial monitors.
 * @note Assumes write() sends a single character to the UART output.
 *       Some systems may require an additional carriage return ('\r') 
 *       for proper line formatting (e.g., "\r\n").
 */
void __UART__::println(void)
{
    this->write((const uint8_t)'\n');
}

/**
 * @brief Prints a single character followed by a newline to the UART.
 * @param c The character to be printed.
 * @details Sends the character 'c' to the UART using write(), 
 *          then calls println() to print a newline character ('\n').
 *          This ensures the output moves to the next line after the character.
 * @note Assumes write() sends a single character to the UART output.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const char c)
{
    this->write((const uint8_t)c);
    this->println();
}

/**
 * @brief Prints a null-terminated string followed by a newline to the UART.
 * @param s Pointer to the null-terminated string to be printed.
 * @details Iterates through the string, sending each character to the UART using write().
 *          Continues until the null terminator ('\0') is reached, indicating the end of the string.
 *          After printing the string, calls println() to print a newline character ('\n').
 * @note Assumes write() sends a single character to the UART output.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const char* s)
{
    while (*s)
        this->write((const uint8_t)*s++);
    this->println();
}

/**
 * @brief Prints a string stored in flash memory followed by a newline to the UART.
 * @param s Reference to a FlashStringHelper object representing the flash-stored string.
 * @details Retrieves a pointer to the flash-stored string using s.get().
 *          Iterates through the string, reading each byte from flash memory with pgm_read_byte().
 *          Sends each byte to the UART using write() until a null terminator ('\0') is encountered.
 *          After printing the string, calls println() to print a newline character ('\n').
 * @note Assumes write() sends a single character to the UART output.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 *       This is optimized for platforms like AVR, where strings are stored in program memory.
 */
void __UART__::println(const FlashStringHelper &s)
{
    const char* ptr = s.get();
    while (pgm_read_byte(ptr))
        this->write((const uint8_t)pgm_read_byte(ptr++));
    this->println();
}

/**
 * @brief Prints an unsigned 8-bit integer followed by a newline to the UART.
 * @param n The uint8_t value to be printed (range 0 to 255).
 * @details Calls print(uint8_t) to convert and send the numeric value as ASCII digits to the UART.
 *          After printing the number, calls println() to print a newline character ('\n').
 * @note Assumes print(uint8_t) handles the conversion of the number to ASCII digits.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const uint8_t n)
{
    this->print(n);
    this->println();
}

/**
 * @brief Prints an unsigned 16-bit integer followed by a newline to the UART.
 * @param n The uint16_t value to be printed (range 0 to 65535).
 * @details Calls print(uint16_t) to convert and send the numeric value as ASCII digits to the UART.
 *          After printing the number, calls println() to print a newline character ('\n').
 * @note Assumes print(uint16_t) handles the conversion of the number to ASCII digits.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const uint16_t n)
{
    this->print(n);
    this->println();
}

/**
 * @brief Prints an unsigned 32-bit integer followed by a newline to the UART.
 * @param n The uint32_t value to be printed (range 0 to 4294967295).
 * @details Calls print(uint32_t) to convert and send the numeric value as ASCII digits to the UART.
 *          After printing the number, calls println() to print a newline character ('\n').
 * @note Assumes print(uint32_t) handles the conversion of the number to ASCII digits.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const uint32_t n)
{
    this->print(n);
    this->println();
}

/**
 * @brief Prints a signed 8-bit integer followed by a newline to the UART.
 * @param n The int8_t value to be printed (range -128 to 127).
 * @details Calls print(int8_t) to convert and send the numeric value as ASCII digits to the UART.
 *          Handles negative numbers with a minus sign ('-') if implemented in print().
 *          After printing the number, calls println() to print a newline character ('\n').
 * @note Assumes print(int8_t) handles the conversion of the number to ASCII digits,
 *       including proper handling of negative values.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const int8_t n)
{
    this->print(n);
    this->println();
}

/**
 * @brief Prints a signed 16-bit integer followed by a newline to the UART.
 * @param n The int16_t value to be printed (range -32,768 to 32,767).
 * @details Calls print(int16_t) to convert and send the numeric value as ASCII digits to the UART.
 *          Handles negative numbers with a minus sign ('-') if implemented in print().
 *          After printing the number, calls println() to print a newline character ('\n').
 * @note Assumes print(int16_t) handles the conversion of the number to ASCII digits,
 *       including proper handling of negative values.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const int16_t n)
{
    this->print(n);
    this->println();
}

/**
 * @brief Prints a signed 32-bit integer followed by a newline to the UART.
 * @param n The int32_t value to be printed (range -2,147,483,648 to 2,147,483,647).
 * @details Calls print(int32_t) to convert and send the numeric value as ASCII digits to the UART.
 *          Handles negative numbers with a minus sign ('-') if implemented in print().
 *          After printing the number, calls println() to print a newline character ('\n').
 * @note Assumes print(int32_t) handles the conversion of the number to ASCII digits,
 *       including proper handling of negative values.
 *       The newline format is '\n'; some systems may require "\r\n" for proper line breaks.
 */
void __UART__::println(const int32_t n)
{
    this->print(n);
    this->println();
}

/**
 * @brief Disables the UART communication and releases associated resources.
 * @details If the UART has been initialized (this->began is true), this function disables the UART.
 *          It first waits for any ongoing transmission to complete, using `isTransmitting()`.
 *          It then flushes the transmission buffer, effectively stopping any buffered data from being sent.
 *          For AVR-based systems (e.g., ATmega328), this function clears control registers and disables the UART.
 *          If the system is not supported (other than AVR-based), it throws an error.
 * @return Returns 1 if UART was successfully disabled, 0 if UART was not started.
 * @note This function is specific to AVR architectures. If used on other platforms, it may result in a compile-time error.
 */
const uint8_t __UART__::end(void)
{
    if (!this->began)
        return (0);

    this->began = 0;
    while (this->isTransmitting());
    this->flush();
    #if defined(__AVR_ATmega328__) || \
        defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    *this->ubrrh = 0;
    *this->ubrrl = 0;
    *this->ucsra = 0;
    *this->ucsrc &= ~((1 << UCSZ01) | \
                      (1 << UCSZ00));
    *this->ucsrb &= ~((1 << RXEN0) | \
                      (1 << RXCIE0) | \
                      (1 << TXEN0) | \
                      (1 << UDRIE0));
    #else
    #error "Can't end the UART bus implementation"
    #endif
    return (1);
}

/**
 * @brief ISR (Interrupt Service Routine) for receiving data on the UART.
 * @details This function is triggered by the UART receive interrupt. It reads the incoming byte from the UART data register (UDR) 
 *          and stores it in the receive buffer (`rxBuffer`). The buffer index (`rxHead`) is then incremented in a circular manner 
 *          using modulo operation to prevent overflow and ensure continuous reception.
 * @note This function is interrupt-driven, meaning it runs automatically when new data is received over UART.
 *       It should be as fast as possible to avoid interrupt delays.
 */
void __UART__::isrRX(void)
{
    this->rxBuffer[this->rxHead] = *this->udr;
    this->rxHead = (this->rxHead + 1) % UART_RX_BUFFER_SIZE;
}

/**
 * @brief ISR (Interrupt Service Routine) for transmitting data on the UART.
 * @details This function is triggered when the UART's transmit buffer is empty and ready to send more data.
 *          If there is data remaining in the transmit buffer (`txBuffer`), it retrieves the next byte to be sent from the `txBuffer`
 *          and writes it to the UART data register (UDR). The `txTail` pointer is then incremented in a circular manner.
 *          If the transmit buffer is empty (i.e., all data has been sent), the UDRIE0 interrupt is disabled to prevent further interrupts 
 *          until new data is available.
 * @note This function ensures that UART data transmission occurs continuously without interruption, as long as there is data in the buffer.
 *       It should be kept fast to avoid delaying the transmission process.
 */
void __UART__::isrUDRE(void)
{
    if (this->txHead != this->txTail)
    {
        *this->udr = this->txBuffer[this->txTail];
        this->txTail = (this->txTail + 1) % UART_TX_BUFFER_SIZE;
    }
    else
    {
        #if defined(__AVR_ATmega328__) || \
            defined(__AVR_ATmega328P__) || \
            defined(__AVR_ATmega328PB__)
        *this->ucsrb &= ~(1 << UDRIE0);
        #else
        #error "Can't turn off the UDRIE interrupt"
        #endif
    }
}
