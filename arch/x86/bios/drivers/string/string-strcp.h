#ifndef STRING_STRCP_H
#define STRING_STRCP_H

#include "lib/baselib.h"

static inline char* strcp(const char* s, char* dst)
{
    char* _d = dst;
    while ((*_d++ = *s++) != '\0'){}
    return dst;
}

static inline char* strncp(const char* s, char* dst, size_t to)
{
    char* _d = dst;
    while ((*_d++ = *s++) != '\0' || to){}
    return dst;
}

#endif