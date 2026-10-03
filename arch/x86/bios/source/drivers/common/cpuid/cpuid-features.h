#ifndef CPUID_FEATURES_H
#define CPUID_FEATURES_H

#include "common/baselib.h"
#include "common/cpuid/cpuid.h"
#include "common/cpuid/cpuid-features-structinfo.h"

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