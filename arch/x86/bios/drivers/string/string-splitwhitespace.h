#ifndef STRING_SPLIT_WHITESPACE_H
#define STRING_SPLIT_WHITESPACE_H

#include "lib/baselib.h"

static inline bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f';
}

static inline char* split_whitespace(char* s)
{
    for (size_t i = 0; s[i] != '\0'; i++)
    {
        if (is_whitespace(s[i])) s[i] = '\0';
    }
    return s;
}

#endif