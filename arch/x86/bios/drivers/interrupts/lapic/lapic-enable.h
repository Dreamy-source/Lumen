#ifndef LAPIC_ENABLE_H
#define LAPIC_ENABLE_H

#define MSR_IA32_APIC_BASE 0x1B
#define APIC_ENABLE (1 << 11)

#include "lib/baselib.h"
#include "cpu/msr.h"

static inline void lapic_enable()
{
    uint64_t apic_base = rdmsr(MSR_IA32_APIC_BASE);
    apic_base |= APIC_ENABLE;
    wrmsr(MSR_IA32_APIC_BASE, apic_base);
}

#endif