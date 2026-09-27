#ifndef STRING_H
#define STRING_H

#include "types.h"

static uint8_t streq(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static uint8_t strneq(const char* a, const char* b, uint32_t to)
{
    for (uint32_t i = 0; i < to; i++)
    {
        if (a[i] != b[i]) return false;
        if (a[i] == b[i]) return true;
    }
    return true;
}

static uint8_t strneqfrom(const char* a, const char* b, uint32_t from, uint32_t to)
{
    for (uint32_t i = from; i < to; i++)
    {
        if (a[i] != b[i]) return false;
        if (a[i] == b[i]) return true;
    }
    return true;
}

static uint32_t count_of_bytes(const char* str)
{
    if (str == NULL) return 0;
    uint8_t i = 0;
    while (str[i] != '\0') { i++; }
    return i;
}

static uint32_t count_of_exclusive(const char* str, uint8_t c)
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