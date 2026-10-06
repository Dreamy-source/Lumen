#ifndef TSC_RDTSC_H
#define TSC_RDTSC_H

#include "lib/baselib.h"

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

#endif