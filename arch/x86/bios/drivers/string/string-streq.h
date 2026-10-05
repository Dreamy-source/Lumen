#ifndef STRING_STREQ_H
#define STRING_STREQ_H

#include "lib/baselib.h"

static bool streq(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b) return false;
        a++;
        b++;
    }
    return *a == *b;
}

static bool strneq(const char* a, const char* b, uint32_t to)
{
    for (uint32_t i = 0; i < to; i++)
    {
        if (a[i] != b[i]) return false;
        if (a[i] == '\0') return true;
    }
    return true;
}

static bool strneqfrom(const char* a, const char* b, uint32_t from, uint32_t to)
{
    for (uint32_t i = from; i < to; i++)
    {
        if (a[i] != b[i]) return false;
        if (a[i] == '\0') return true;
    }
    return true;
}

#endif