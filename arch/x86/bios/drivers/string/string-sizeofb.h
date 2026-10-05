#ifndef STRING_SIZEOFB_H
#define STRING_SIZEOFB_H

#include "lib/baselib.h"

static size_t sizeofb(const char* str)
{
    if (str == NULL) return false;
    uint8_t i = 0;
    while (str[i] != '\0') { i++; }
    return i;
}

// size of bytes in stroke exclude char
static size_t sizeofb_exchr(const char* str, char c)
{
    if (str == NULL) return false;
    uint8_t i = 0;
    while (str[i] != '\0' || c) { i++; }
    return i;
}

#endif