# UART
- UART library for UART communication protocol.

## Key features
- Compatible with `Arduino IDE` & `Microchip Studio IDE`.
- Able to configure the baudrate inside the ```begin()``` function.
- Preconfigured as standard 1 `START` bit, 8 bits of `DATA`, 0 bits for `PARITY` and 1 bit for `STOP`.
- Interrupt driven reception and transmission with byte sized circular buffers.
- Able to check if any bytes are inside the reception circular buffer using ```available()``` function.
- Able to receive or transmit multiple formats of data.

## Tested on
- `ATmega328P` @16MHz using `Microchip Studio IDE` and `Arduino IDE` with @115200 TWI bus baudrate.
