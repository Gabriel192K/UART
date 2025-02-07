#include <UART.h>

/**
 * @brief Setup function for initializing UART and sending test data over UART.
 * 
 * This function initializes the UART0 at a baud rate of 115200 and sends various test
 * messages, including characters, strings, and different numeric types, over UART to 
 * the connected serial monitor.
 */
void setup(void)
{
    /**
     * @brief Initialize UART0 with a baud rate of 115200.
     * 
     * This sets up the UART communication at 115200 baud for serial communication.
     */
    UART0.begin(115200);

    /**
     * @brief Send "UART - Test Tx" message over UART.
     * 
     * This sends a simple message to the serial monitor indicating the start of UART
     * transmission.
     */
    UART0.println(F("UART - Test Tx"));

    /**
     * @brief Send a character from RAM ('c') over UART.
     * 
     * This sends the character 'c' from RAM over UART to the serial monitor.
     */
    UART0.print(F("Character from RAM: ")); 
    UART0.println('c');

    /**
     * @brief Send a string from RAM over UART.
     * 
     * This sends the string "Hello from RAM" over UART to the serial monitor.
     */
    UART0.print(F("String from RAM: ")); 
    UART0.println("Hello from RAM");

    /**
     * @brief Send a string from Flash memory over UART.
     * 
     * This sends the string "Hello from Flash" over UART to the serial monitor.
     */
    UART0.print(F("String from Flash: ")); 
    UART0.println("Hello from Flash");

    /**
     * @brief Send an 8-bit unsigned number (UINT8_MAX) over UART.
     * 
     * This sends the maximum value of an 8-bit unsigned integer (255) over UART.
     */
    UART0.print(F("8 bit unsigned number: ")); 
    UART0.println((const uint8_t)UINT8_MAX);

    /**
     * @brief Send a 16-bit unsigned number (UINT16_MAX) over UART.
     * 
     * This sends the maximum value of a 16-bit unsigned integer (65535) over UART.
     */
    UART0.print(F("16 bit unsigned number: ")); 
    UART0.println((const uint16_t)UINT16_MAX);

    /**
     * @brief Send a 32-bit unsigned number (UINT32_MAX) over UART.
     * 
     * This sends the maximum value of a 32-bit unsigned integer (4294967295) over UART.
     */
    UART0.print(F("32 bit unsigned number: ")); 
    UART0.println((const uint32_t)UINT32_MAX);

    /**
     * @brief Send an 8-bit signed number (INT8_MIN) over UART.
     * 
     * This sends the minimum value of an 8-bit signed integer (-128) over UART.
     */
    UART0.print(F("8 bit signed number: ")); 
    UART0.println((const int8_t)INT8_MIN);

    /**
     * @brief Send a 16-bit signed number (INT16_MIN) over UART.
     * 
     * This sends the minimum value of a 16-bit signed integer (-32768) over UART.
     */
    UART0.print(F("16 bit signed number: ")); 
    UART0.println((const int16_t)INT16_MIN);

    /**
     * @brief Send a 32-bit signed number (INT32_MIN) over UART.
     * 
     * This sends the minimum value of a 32-bit signed integer (-2147483648) over UART.
     */
    UART0.print(F("32 bit signed number: ")); 
    UART0.println((const int32_t)INT32_MIN);
}

/**
 * @brief Main loop function.
 * 
 * This function is empty in the current code. It serves as the main loop of the program
 * where continuous tasks (such as reading from UART or responding to commands) can be
 * added in the future. For now, it runs once the setup function is complete.
 */
void loop(void)
{
    // Empty loop; future functionality can be added here
}
