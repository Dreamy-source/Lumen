#ifndef LAPIC_SVR_H
#define LAPIC_SVR_H

#define LAPIC_SVR_ENABLE        (1 << 8)
#define LAPIC_SVR_SPURIOUS_VEC  0xFF

#include "lib/baselib.h"
#include "acpi/madt/acpi-madt-structinfo.h"
#include "acpi/acpi-locate-t.h"
#include "interrupts/lapic/lapic-init.h"

// spurious vector register
static inline void lapic_wrsvr(uint32_t val)
{
    // bit 8 - enable
    g_lapic[0xF0 / 4] = val;
}

#endif