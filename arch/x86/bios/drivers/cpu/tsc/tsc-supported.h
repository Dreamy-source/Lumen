#ifndef TSC_SUPPORTED_H
#define TSC_SUPPORTED_H

#include "lib/baselib.h"
#include "cpu/cpuid/cpuid.h"

static inline void tsc_supported()
{
    uint32_t eax, ebx, ecx, edx;
    cpuid(0x01, &eax, &ebx, &ecx, &edx);
    if (!(ecx & (1 << 24))) {
        kprintf("[APIC] TSC-Deadline not supported\n", 0x07, 0x0F);
        return;
    }
}

#endif