#ifndef __FLASH_STRING_HELPER_H__
#define __FLASH_STRING_HELPER_H__

/* Dependencies */
#include <stdio.h>
#include <avr/pgmspace.h>

class FlashStringHelper
{
    public:
        FlashStringHelper(const char* str);
        ~FlashStringHelper();
        const char* get(void);
    private:
        const char* ptr;
};

#define F(string_literal) (FlashStringHelper(PSTR(string_literal)))

#endif
