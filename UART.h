#ifndef __UART_H__
#define __UART_H__

/* Dependencies */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/atomic.h>

/* Macros */
#define UART_RX_BUFFER_SIZE (const uint8_t)64
#define UART_TX_BUFFER_SIZE (const uint8_t)64

class __UART__
{
    public:
        __UART__(volatile uint8_t* ubrrh, \
                 volatile uint8_t* ubrrl, \
                 volatile uint8_t* ucsra, \
                 volatile uint8_t* ucsrb, \
                 volatile uint8_t* ucsrc, \
                 volatile uint8_t* udr);
        ~__UART__();
        const uint8_t begin         (const uint32_t baudrate);
        const uint8_t available     (void);
        void          flush         (void);
        const uint8_t isTransmitting(void);
        const uint8_t read          (void);
        void          write         (const uint8_t n);
        void          write         (const uint8_t* n, const uint8_t size);
        void          write         (const void* n, const uint8_t size);
        const uint8_t end           (void);
        void          isrRX         (void);
        void          isrUDRE       (void);
    private:
        volatile uint8_t* ubrrh;
        volatile uint8_t* ubrrl;
        volatile uint8_t* ucsra;
        volatile uint8_t* ucsrb;
        volatile uint8_t* ucsrc;
        volatile uint8_t* udr;
        volatile uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
        volatile uint8_t txBuffer[UART_TX_BUFFER_SIZE];
        volatile uint8_t rxHead, rxTail, txHead, txTail;
        uint8_t began;
};

#if defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__)
#define HAS_UART0
extern __UART__ UART;
#else
#error "Error! UART implementation is not available for this MCU."
#endif

#endif
