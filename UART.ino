#include "UART.h"

void setup(void)
{
    UART.begin(115200);
    UART.write('A');
}

void loop(void)
{
    if (UART.available())
        UART.write(UART.read());
}
