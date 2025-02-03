/* Dependencies */
#include "FlashStringHelper.h"

/*!
 * @brief  FlashStringHelper constructor
 * @param  str
 *         The pointer to the string stored in flash memory
 */
FlashStringHelper::FlashStringHelper(const char* str)
{
    this->ptr = str;
}

/*!
 * @brief  FlashStringHelper destructor
 */
FlashStringHelper::~FlashStringHelper()
{
    this->ptr = NULL;
}

/*!
 * @brief  Getting the pointer to the string stored in flash memory
 * @return Returns the apointer to the string stored in flash memory
 */
const char* FlashStringHelper::get(void)
{
    return (this->ptr);
}
