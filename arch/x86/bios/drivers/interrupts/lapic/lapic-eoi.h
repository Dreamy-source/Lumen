#ifndef LAPIC_EOI_H
#define LAPIC_EOI_H

#define LAPIC_EOI_RESET_ALL_OLD_INTERRUPTS 0

#include "lib/baselib.h"
#include "acpi/madt/acpi-madt-structinfo.h"
#include "acpi/acpi-locate-t.h"
#include "interrupts/lapic/lapic-init.h"

// end of interrupt
static inline void lapic_wreoi(uint8_t val)
{
    g_lapic[0xB0 / 4] = val;
}

#endif