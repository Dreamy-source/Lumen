#ifndef STRING_H
#define STRING_H

#include "common/types.h"

static bool streq(const char* a, const char* b)
{
    while (*a && *b)
    {
        if (*a != *b) return 0;
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

static size_t sizeofb(const char* str)
{
    if (str == NULL) return 0;
    uint8_t i = 0;
    while (str[i] != '\0') { i++; }
    return i;
}

// size of bytes in stroke exclude char
static size_t sizeofb_exchr(const char* str, char c)
{
    if (str == NULL) return 0;
    uint8_t i = 0;
    while (str[i] != '\0' || c) { i++; }
    return i;
}

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

static inline void* memset(void* s, int c, size_t n)
{
    uint8_t* _s = (uint8_t*)s;
    while (n--) *_s++ = (uint8_t)c;
    return s;
}

static inline void* memcp(void* dst, const void* src, size_t n)
{
    uint8_t* d = (uint8_t*)dst;
    const uint8_t* s = (uint8_t*)src;
    while (n--) *d++ = *s++;
    return dst;
}

static inline int memcmp(const void* a, const void* b, size_t n)
{
    const uint8_t* pa = (const uint8_t*)a;
    const uint8_t* pb = (const uint8_t*)a;
    while (n--) {
        if (*pa != *pb) return (int)*pa - (int)*pb;
        pa++; pb++;
    }
    return 0;
}

static inline void strcat(const char* a, const char* b)
{
    // write
}

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