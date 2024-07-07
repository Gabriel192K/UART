#include <UART.h>

void setup(void)
{
    UART.begin(115200);
}

void loop(void)
{
    const uint8_t character = 'A';
    const uint8_t array[5] = {'H', 'e', 'l', 'l', 'o'};
    struct Structure
    {
        const uint8_t array[5] = {'W', 'o', 'r', 'l', 'd'};
        const uint8_t character = '\n';
    };
    Structure structure;
    UART.write(character);
    UART.write(array, sizeof(array) / sizeof(array[0]));
    UART.write(&structure, sizeof(structure));
    delay(1000);
}
