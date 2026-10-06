#ifndef LAPIC_ESR_H
#define LAPIC_ESR_H

#define LAPIC_ESR_CLEAR_ALL_ERROR_REGISTER 0

#include "lib/baselib.h"
#include "acpi/madt/acpi-madt-structinfo.h"
#include "acpi/acpi-locate-t.h"
#include "interrupts/lapic/lapic-init.h"

// error status register
static inline void lapic_wresr(uint8_t val0, uint8_t val1)
{
    g_lapic[0x280 / 4] = val0;
    g_lapic[0x280 / 4] = val1;
}

#endif