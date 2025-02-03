#ifndef __UART_H__
#define __UART_H__

/* Dependencies */
#include <stdio.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/atomic.h>
#include "FlashStringHelper.h"

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
        void          read          (uint8_t* n, const uint8_t size);
        void          read          (void* n, const uint8_t size);
        void          write         (const uint8_t n);
        void          write         (const uint8_t* n, const uint8_t size);
        void          write         (const void* n, const uint8_t size);
        void          print         (const char c);
        void          print         (const char* s);
        void          print         (const FlashStringHelper &s);
        void          print         (const uint8_t n);
        void          print         (const uint16_t n);
        void          print         (const uint32_t n);
        void          print         (const int8_t n);
        void          print         (const int16_t n);
        void          print         (const int32_t n);
        void          println       (void);
        void          println       (const char c);
        void          println       (const char* s);
        void          println       (const FlashStringHelper &s);
        void          println       (const uint8_t n);
        void          println       (const uint16_t n);
        void          println       (const uint32_t n);
        void          println       (const int8_t n);
        void          println       (const int16_t n);
        void          println       (const int32_t n);
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

#if defined(__AVR_ATmega328__)  || \
    defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__)
    extern __UART__ UART0;
#endif

#if defined(__AVR_ATmega328PB__)
    extern __UART__ UART1;
#endif



#endif