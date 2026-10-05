#ifndef STRING_MEM_H
#define STRING_MEM_H

#include "lib/baselib.h"

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
    return false;
}

#endif