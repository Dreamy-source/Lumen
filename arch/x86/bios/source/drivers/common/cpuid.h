#ifndef CPUID_H
#define CPUID_H

#define VENDOR_ID_BYTE_0_15  0x80000002
#define VENDOR_ID_BYTE_16_31 0x80000003
#define VENDOR_ID_BYTE_32_47 0x80000004

#include "common/port.h"
#include "common/types.h"
#include "video/vga.h"

typedef struct {
    bool fpu;
    bool sse;
    bool sse2;
    bool avx;
    bool vmx;
    bool mmx;
    bool sse3;
    bool ssse3;
    bool sse41;
    bool sse42;
    bool x86_64;
    bool nx;
    bool rdrand;
    bool aesni;
    bool sha;
    bool syscall;
    bool p1gb;
    bool p2mb;
} cpu_features_t;

static inline void cpuid(uint32_t code, uint32_t* eax, uint32_t* ebx, uint32_t* ecx, uint32_t* edx)
{
    __asm__ volatile ( "cpuid" : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx) : "a"(code), "c"(0));
}

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

static inline cpu_features_t get_cpu_features()
{
    cpu_features_t f = {0};
    
    uint32_t eax, ebx, ecx, edx;
    cpuid(1, &eax, &ebx, &ecx, &edx);

    f.fpu    = (edx & (1 << 0))  != 0;
    f.vmx    = (ecx & (1 << 5))  != 0;
    f.mmx    = (edx & (1 << 23)) != 0;
    f.sse    = (edx & (1 << 25)) != 0;
    f.sse2   = (edx & (1 << 26)) != 0;
    f.sse3   = (ecx & (1 << 0))  != 0;
    f.ssse3  = (ecx & (1 << 9))  != 0;
    f.sse41  = (ecx & (1 << 19)) != 0;
    f.sse42  = (ecx & (1 << 20)) != 0;
    f.avx    = (ecx & (1 << 28)) != 0;

    cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
    f.x86_64 = (edx & (1 << 29)) != 0;
    f.nx     = (edx & (1 << 20)) != 0;

    f.rdrand  = (ecx & (1 << 30)) != 0;
    f.aesni   = (ecx & (1 << 25)) != 0;
    f.sha     = (ebx & (1 << 29)) != 0;
    f.syscall = (edx & (1 << 11)) != 0;
    f.p1gb    = (edx & (1 << 26)) != 0;
    f.p2mb    = (edx & (1 << 26)) != 0;

    return f;
}

#endif