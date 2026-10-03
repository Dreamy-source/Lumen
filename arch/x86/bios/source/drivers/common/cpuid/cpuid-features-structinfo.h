#ifndef CPU_FEATURES_STRUCTINFO_H
#define CPU_FEATURES_STRUCTINFO_H

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

#endif