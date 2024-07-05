/* Dependencies */
#include "UART.h"

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

__UART__::~__UART__()
{
    this->ubrrh = NULL;
    this->ubrrl = NULL;
    this->ucsra = NULL;
    this->ucsrb = NULL;
    this->ucsrc = NULL;
    this->udr = NULL;
}

const uint8_t __UART__::begin(const uint32_t baudrate)
{
    if (this->began)
        return (0);

    this->began = 1;
    uint16_t prescale = (F_CPU / 4 / baudrate - 1) / 2;
    
    #if defined(__AVR_ATmega328P__) || \
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
    #endif
    return (1);
}

const uint8_t __UART__::available(void)
{
    uint8_t bytes = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        bytes = (UART_RX_BUFFER_SIZE + this->rxHead - this->rxTail) % UART_RX_BUFFER_SIZE;
    return (bytes);
}

void __UART__::flush(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
        this->rxHead = this->rxTail;
}

const uint8_t __UART__::isTransmitting(void)
{
    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    return ((*this->ucsrb & (1 << UDRIE0)) != 0);
    #endif
}

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

void __UART__::write(const uint8_t n)
{
    const uint8_t head = (this->txHead + 1) % UART_TX_BUFFER_SIZE;
    while (head == this->txTail);

    this->txBuffer[this->txHead] = n;
    this->txHead = head;

    #if defined(__AVR_ATmega328P__) || \
        defined(__AVR_ATmega328PB__)
    *this->ucsrb = *this->ucsrb | (1 << UDRIE0);
    #endif

}

void __UART__::write(const uint8_t* n, const uint8_t size)
{
    for (const uint8_t* p = n; p < (n + size); p++)
        this->write(*p);
}

void __UART__::write(const void* n, const uint8_t size)
{
    this->write((const uint8_t*)n, size);
}

const uint8_t __UART__::end(void)
{
    if (!this->began)
        return (0);

    this->began = 0;
    while (this->isTransmitting());
    this->flush();
    #if defined(__AVR_ATmega328P__) || \
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
    #endif
    return (1);
}

void __UART__::isrRX(void)
{
    this->rxBuffer[this->rxHead] = *this->udr;
    this->rxHead = (this->rxHead + 1) % UART_RX_BUFFER_SIZE;
}

void __UART__::isrUDRE(void)
{
    if (this->txHead != this->txTail)
    {
        *this->udr = this->txBuffer[this->txTail];
        this->txTail = (this->txTail + 1) % UART_TX_BUFFER_SIZE;
    }
    else
    {
        #if defined(__AVR_ATmega328P__) || \
            defined(__AVR_ATmega328PB__)
        *this->ucsrb &= ~(1 << UDRIE0);
        #endif
    }
}