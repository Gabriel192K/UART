/* Dependencies */
#include "UART.h"

/*!
 * @brief  __UART0__ constructor
 * @param  ubrrh
 *         The UART baudrate high address
 * @param  ubrrl
 *         The UART baudrate low address
 * @param  ucsra
 *         The UART control and status register A address
 * @param  ucsrb
 *         The UART control and status register B address
 * @param  ucsrc
 *         The UART control and status register C address
 * @param  udr
 *         The UART data register address
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

/*!
 * @brief  __UART0__ destructor
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

/*!
 * @brief  Starting UART bus implementation
 * @param  baudrate
 *         The baudrate of UART bus
 * @return False if already started this UART bus, otherwise true
 */
const uint8_t __UART__::begin(const uint32_t baudrate)
{
    if (this->began)
        return (0);

    this->began = 1;

    sei(); // Turn on global interrupts

    uint16_t prescale = (F_CPU / 4 / baudrate - 1) / 2;
    
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
    
    *this->ubrrh = (uint8_t)(prescale >> 8); // Write <LSB> of the prescale
    *this->ubrrl = (uint8_t)prescale;        //  Write <MSB> of the prescale
    *this->ucsrc |= (1 << UCSZ01) | \
                    (1 << UCSZ00);           // 8 bit data transmission size
    *this->ucsrb |= (1 << RXEN0) | \
                    (1 << RXCIE0) | \
                    (1 << TXEN0);            // Enable <RX>, <RX-IRQ>, <TX>
    #else
        #error "Can not config UART bus in begin()"
    #endif
    return (1);
}

/*!
 * @brief  Checking how many bytes the RX circular buffer has
 * @return The number of bytes the RX circular buffer has
 */
const uint8_t __UART__::available(void)
{
    uint8_t bytes = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        bytes = (UART_RX_BUFFER_SIZE + this->rxHead - this->rxTail) % UART_RX_BUFFER_SIZE;
    return (bytes);
}

/*!
 * @brief  Flushing the RX circular buffer
 */
void __UART__::flush(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        this->rxHead = this->rxTail;
}

/*!
 * @brief  Checking if the UDRE interrupt is enabled aka is transmitting
 * @return Flag if UDRE interrupt is enabled
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

/*!
 * @brief  Reading a byte from RX circular buffer
 * @return Byte from RX circular buffer
 */
const uint8_t __UART__::read(void)
{
    if (this->rxHead == this->rxTail) // Check if RX buffer is empty
        return (0);
	
	uint8_t byte = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        byte = this->rxBuffer[this->rxTail];
        this->rxTail = (this->rxTail + 1) % UART_RX_BUFFER_SIZE;
    }
    return (byte);
}

/*!
 * @brief  Reading an amount of bytes from RX circular buffer into an array
 * @param  n
 *         The array of bytes to be filled
 * @param  size
 *         The size of the array of bytes to be filled
 */
void __UART__::read(uint8_t* n, const uint8_t size)
{
    for (uint8_t i = 0; i < size;)
        if (this->available())
            n[i++] = this->read();
}

/*!
 * @brief  Reading an amount of bytes from RX circular buffer into a type-unknown zone of memory
 * @param  n
 *         The type-unknown zone of memory to be filled
 * @param  size
 *         The size of the type-unknown zone of memory to be filled
 */
void __UART__::read(void* n, const uint8_t size)
{
    this->read((uint8_t*)n, size);
}

/*!
 * @brief  Writing a byte into the TX circular buffer
 * @param  n
 *         The byte to be written into the TX circular buffer
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

/*!
 * @brief  Writing an amount of bytes from an array of bytes into the TX circular buffer
 * @param  n
 *         The array of bytes to be written into the TX circular buffer
 * @param  size
 *         The size of the array of bytes to be written into the TX circular buffer
 */
void __UART__::write(const uint8_t* n, const uint8_t size)
{
    for (const uint8_t* p = n; p < (n + size); p++)
        this->write(*p);
}

/*!
 * @brief  Writing an amount of bytes from a type-unknown zone of memory into the TX circular buffer
 * @param  n
 *         The type-unknown zone of memory to be written into the TX circular buffer
 * @param  size
 *         The size of the a type-unknown zone of memory to be written into the TX circular buffer
 */
void __UART__::write(const void* n, const uint8_t size)
{
    this->write((const uint8_t*)n, size);
}

/*!
 * @brief  Writing an ASCII format byte into the TX circular buffer
 * @param  c
 *         The ASCII format byte to be written into the TX circular buffer
 */
void __UART__::print(const char c)
{
    this->write((const uint8_t)c);
}

/*!
 * @brief  Writing an ASCII format array of bytes into the TX circular buffer
 * @param  s
 *         The ASCII format byte array to be written into the TX circular buffer
 */
void __UART__::print(const char* s)
{
    while (*s)
        this->write((const uint8_t)*s++);
}

/*!
 * @brief  Writing an ASCII format array of bytes into the TX circular buffer stored into flash memory
 * @param  s
 *         The ASCII format byte array to be written into the TX circular buffer stored into flash memory
 */
void __UART__::print(const FlashStringHelper &s)
{
    const char* ptr = s.get();
    while (pgm_read_byte(ptr))
        this->write((const uint8_t)pgm_read_byte(ptr++));
}

/*!
 * @brief  Writing an ASCII format byte into the TX circular buffer
 * @param  n
 *         The ASCII format byte to be written into the TX circular buffer
 */
void __UART__::print(const uint8_t n)
{
    if (n > 99) this->print((const char)(((n / 100) % 10) + '0'));
    if (n > 9)  this->print((const char)(((n / 10) % 10) + '0'));
    this->print((const char)((n % 10) + '0'));
}

/*!
 * @brief  Writing an ASCII format word into the TX circular buffer
 * @param  n
 *         The ASCII format word to be written into the TX circular buffer
 */
void __UART__::print(const uint16_t n)
{
    if (n > 9999) this->print((const char)(((n / 10000) % 10) + '0'));
    if (n > 999)  this->print((const char)(((n / 1000) % 10) + '0'));
    if (n > 99)   this->print((const char)(((n / 100) % 10) + '0'));
    if (n > 9)    this->print((const char)(((n / 10) % 10) + '0'));
    this->print((const char)((n % 10) + '0'));
}

/*!
 * @brief  Writing an ASCII format dword into the TX circular buffer
 * @param  n
 *         The ASCII format dword to be written into the TX circular buffer
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

/*!
 * @brief  Writing an signed ASCII format byte into the TX circular buffer
 * @param  n
 *         The signed ASCII format byte to be written into the TX circular buffer
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

/*!
 * @brief  Writing an signed ASCII format word into the TX circular buffer
 * @param  n
 *         The signed ASCII format word to be written into the TX circular buffer
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

/*!
 * @brief  Writing an signed ASCII format dword into the TX circular buffer
 * @param  n
 *         The signed ASCII format dword to be written into the TX circular buffer
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

/*!
 * @brief  Writing an ASCII format "new line" byte into the TX circular buffer
 * @param  c
 *         The ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(void)
{
    this->write((const uint8_t)'\n');
}

/*!
 * @brief  Writing an ASCII format byte followed by an ASCII format "new line" byte into the TX circular buffer 
 * @param  c
 *         The ASCII format byte followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const char c)
{
    this->write((const uint8_t)c);
    this->println();
}

/*!
 * @brief  Writing an ASCII format array of bytes followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  s
 *         The ASCII format byte array followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const char* s)
{
    while (*s)
        this->write((const uint8_t)*s++);
    this->println();
}

/*!
 * @brief  Writing an ASCII format array of bytes followed by an ASCII format "new line" byte into the TX circular buffer stored into flash memory
 * @param  s
 *         The ASCII format byte array followed by an ASCII format "new line" byte to be written into the TX circular buffer stored into flash memory
 */
void __UART__::println(const FlashStringHelper &s)
{
    const char* ptr = s.get();
    while (pgm_read_byte(ptr))
        this->write((const uint8_t)pgm_read_byte(ptr++));
    this->println();
}

/*!
 * @brief  Writing an ASCII format byte followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  n
 *         The ASCII format byte followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const uint8_t n)
{
    this->print(n);
    this->println();
}

/*!
 * @brief  Writing an ASCII format word followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  n
 *         The ASCII format word followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const uint16_t n)
{
    this->print(n);
    this->println();
}

/*!
 * @brief  Writing an ASCII format dword followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  n
 *         The ASCII format dword followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const uint32_t n)
{
    this->print(n);
    this->println();
}

/*!
 * @brief  Writing an signed ASCII format byte followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  n
 *         The signed ASCII format byte followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const int8_t n)
{
    this->print(n);
    this->println();
}

/*!
 * @brief  Writing an signed ASCII format word followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  n
 *         The signed ASCII format word followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const int16_t n)
{
    this->print(n);
    this->println();
}

/*!
 * @brief  Writing an signed ASCII format dword followed by an ASCII format "new line" byte into the TX circular buffer
 * @param  n
 *         The signed ASCII format dword followed by an ASCII format "new line" byte to be written into the TX circular buffer
 */
void __UART__::println(const int32_t n)
{
    this->print(n);
    this->println();
}

/*!
 * @brief  Ending UART bus implementation
 * @return False if already ended this UART bus, otherwise true
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

/*!
 * @brief  The RX ISR wrapper that will run iside the RX ISR
 */
void __UART__::isrRX(void)
{
    this->rxBuffer[this->rxHead] = *this->udr;
    this->rxHead = (this->rxHead + 1) % UART_RX_BUFFER_SIZE;
}

/*!
 * @brief  The UDRE ISR wrapper that will run iside the UDRE ISR
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
