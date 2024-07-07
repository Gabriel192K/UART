# UART
UART library for handling UART communication

## Key features
- Compatible with `Arduino IDE` & `Microchip Studio IDE`
- Easy ```begin()``` and ```end()``` functions to start or stop UART bus implementation
- Configurable baudrate as a parameter of ```begin()``` function
- Interrupt driven RX and TX using fixed size circular buffers
- Able to check how many bytes are available inside the circular RX buffer
- Able to flush RX using ```flush()``` or check if TX is underway using ```isTransmitting()```
- Able to read or write any type of data sending everything byte by byte onto the UART bus

## Tested on
- ATmega328P @16MHz with `Arduino IDE` & `Microchip Studio IDE` @115200bps
- ATmega328PB @16MHz with `Arduino IDE` & `Microchip Studio IDE` @115200bps
