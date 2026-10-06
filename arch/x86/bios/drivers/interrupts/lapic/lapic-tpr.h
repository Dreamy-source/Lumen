#ifndef LAPIC_TPR_H
#define LAPIC_TPR_H

#define LAPIC_TPR_ALLOW_ALL_PRIORITY_INTERRUPTS 0

#include "lib/baselib.h"
#include "acpi/madt/acpi-madt-structinfo.h"
#include "acpi/acpi-locate-t.h"
#include "interrupts/lapic/lapic-init.h"

// task priority register
static inline void lapic_wrtpr(uint8_t val)
{
    g_lapic[0x80 / 4] = val;
}

#endif