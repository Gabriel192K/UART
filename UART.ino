#include "UART.h"

void setup(void)
{
    UART.begin(115200);
    UART.write('A');
}

void loop(void)
{
    uint8_t buffer[4] = {0, 0, 0, 0};
    UART.read(buffer, sizeof(buffer) / sizeof(buffer[0]));
    UART.write(buffer, sizeof(buffer) / sizeof(buffer[0]));
}
