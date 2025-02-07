/* Dependencies */
#include "FlashStringHelper.h"

/**
 * @brief Constructs a FlashStringHelper object with a pointer to a string stored in flash memory.
 * @param str A pointer to the null-terminated string stored in flash memory (PROGMEM).
 * @details This constructor initializes the `ptr` member with the provided string pointer, 
 *          allowing the object to reference strings stored in flash memory, which helps save RAM.
 * @note Useful for handling constant strings in embedded systems where RAM is limited.
 */
FlashStringHelper::FlashStringHelper(const char* str)
{
    this->ptr = str;
}

/**
 * @brief Destructor for the FlashStringHelper class.
 * @details This destructor resets the internal pointer `ptr` to `NULL` to ensure 
 *          it no longer references the flash memory. Since the class only holds 
 *          a reference to the flash-stored string (without dynamic memory allocation), 
 *          no additional cleanup is required.
 * @note This destructor helps maintain good programming practices by explicitly 
 *       clearing the pointer, even though it's not strictly necessary in this case.
 */
FlashStringHelper::~FlashStringHelper()
{
    this->ptr = NULL;
}

/**
 * @brief Retrieves the pointer to the flash-stored string.
 * @return A constant character pointer (`const char*`) pointing to the string in flash memory.
 * @details This method returns the internal pointer `ptr` that references the 
 *          string stored in flash memory, allowing read-only access to the data.
 * @note The returned pointer should be used with functions compatible with 
 *       flash memory (e.g., `pgm_read_byte` for AVR-based systems).
 */
const char* FlashStringHelper::get(void)
{
    return (this->ptr);
}
