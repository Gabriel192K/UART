#ifndef __FLASH_STRING_HELPER_H__
#define __FLASH_STRING_HELPER_H__

/* Dependencies */
#include <stdio.h>
#include <avr/pgmspace.h>

/**
 * @class FlashStringHelper
 * @brief A helper class for managing strings stored in flash memory (PROGMEM).
 * @details This class provides a simple interface to handle constant strings 
 *          stored in flash memory, reducing RAM usage in embedded systems. 
 *          It stores a pointer to the flash-stored string and provides 
 *          methods to access it safely.
 * 
 * @note Especially useful in memory-constrained environments like AVR microcontrollers,
 *       where RAM is limited but flash memory is abundant.
 */
class FlashStringHelper
{
    public:
        /**
         * @brief Constructs a FlashStringHelper object with a pointer to a string stored in flash memory.
         * @param str A pointer to the null-terminated string stored in flash memory (PROGMEM).
         */
        FlashStringHelper(const char* str);

        /**
         * @brief Destructor for the FlashStringHelper class.
         */
        ~FlashStringHelper();

        /**
         * @brief Retrieves the pointer to the flash-stored string.
         * @return A constant character pointer (`const char*`) pointing to the string in flash memory.
         */
        const char* get(void);

    private:
        const char* ptr;  /**< Pointer to the null-terminated string stored in flash memory. */
};

/**
 * @def F
 * @brief Macro to store a string literal in flash memory and create a FlashStringHelper object.
 * @param string_literal The string literal to be stored in flash memory (PROGMEM).
 * @return A FlashStringHelper object initialized with a pointer to the flash-stored string.
 * 
 * @details This macro wraps the `PSTR()` function, which stores the given string literal
 *          in flash memory instead of RAM. It then creates a `FlashStringHelper` object
 *          to manage and access the flash-stored string efficiently.
 * 
 * @note Using this macro helps save RAM in memory-constrained environments like AVR microcontrollers.
 *       The resulting string should be accessed using flash-compatible functions (e.g., `pgm_read_byte`).
 * 
 * @example
 * // Example usage:
 * uart.println(F("Hello, world!")); // "Hello, world!" is stored in flash memory, not RAM
 */
#define F(string_literal) (FlashStringHelper(PSTR(string_literal)))

#endif
