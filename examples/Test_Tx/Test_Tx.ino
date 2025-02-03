#include <UART.h>

void setup(void)
{
    UART0.begin(115200);
    UART0.println(F("UART - Test Tx"));
    UART0.print(F("Character from RAM: ")); UART0.println('c');
    UART0.print(F("String from RAM: ")); UART0.println("Hello from RAM");
    UART0.print(F("String from Flash: ")); UART0.println("Hello from Flash");
    UART0.print(F("8 bit unsigned number: ")); UART0.println((const uint8_t)UINT8_MAX);
    UART0.print(F("16 bit unsigned number: ")); UART0.println((const uint16_t)UINT16_MAX);
    UART0.print(F("32 bit unsigned number: ")); UART0.println((const uint32_t)UINT32_MAX);
    UART0.print(F("8 bit signed number: ")); UART0.println((const int8_t)INT8_MIN);
    UART0.print(F("16 bit signed number: ")); UART0.println((const int16_t)INT16_MIN);
    UART0.print(F("32 bit signed number: ")); UART0.println((const int32_t)INT32_MIN);
}

void loop(void)
{

}
