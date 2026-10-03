#ifndef CPUID_VENDOR_H
#define CPUID_VENDOR_H

#define VENDOR_ID_BYTE_0_15  0x80000002
#define VENDOR_ID_BYTE_16_31 0x80000003
#define VENDOR_ID_BYTE_32_47 0x80000004

#include "common/baselib.h"
#include "common/cpuid/cpuid.h"

static inline void get_cpu_vendor_part0(char* buf_writeloc)
{
    uint32_t eax, ebx, ecx, edx;
    uint32_t* ptr = (uint32_t*)buf_writeloc;

    cpuid(VENDOR_ID_BYTE_0_15, &eax, &ebx, &ecx, &edx);
    ptr[0] = eax;
    ptr[1] = ebx;
    ptr[2] = ecx;
    ptr[3] = edx;

    buf_writeloc[48] = '\0';
}

static inline void get_cpu_vendor_part1(char* buf_writeloc)
{
    uint32_t eax, ebx, ecx, edx;
    uint32_t* ptr = (uint32_t*)buf_writeloc;

    cpuid(VENDOR_ID_BYTE_16_31, &eax, &ebx, &ecx, &edx);
    ptr[4] = eax;
    ptr[5] = ebx;
    ptr[6] = ecx;
    ptr[7] = edx;

    buf_writeloc[48] = '\0';
}

static inline void get_cpu_vendor_part2(char* buf_writeloc)
{
    uint32_t eax, ebx, ecx, edx;
    uint32_t* ptr = (uint32_t*)buf_writeloc;

    cpuid(VENDOR_ID_BYTE_32_47, &eax, &ebx, &ecx, &edx);
    ptr[8] = eax;
    ptr[9] = ebx;
    ptr[10] = ecx;
    ptr[11] = edx;

    buf_writeloc[48] = '\0';
}

static inline void get_cpu_vendor(char* buf_writeloc)
{
    get_cpu_vendor_part0(buf_writeloc);
    get_cpu_vendor_part1(buf_writeloc);
    get_cpu_vendor_part2(buf_writeloc);
}

#endif