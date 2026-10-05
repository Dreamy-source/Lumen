#ifndef STRING_SC_IN_S_H
#define STRING_SC_IN_S_H

#include "lib/baselib.h"

// count of similiar chars in stroke
static size_t sc_in_s(const char* str, uint8_t c)
{
    uint32_t i = 0;
    while (*str)
    {
        if (*str == c) i++;
        str++;
    }
    return i;
}

#endif