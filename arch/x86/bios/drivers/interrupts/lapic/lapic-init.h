#ifndef LAPIC_INIT_H
#define LAPIC_INIT_H

#include "lib/baselib.h"
#include "acpi/madt/acpi-madt-structinfo.h"
#include "acpi/acpi-locate-t.h"

static volatile uint32_t *g_lapic;
static inline void lapic_init(void) {
    acpi_madt_t *madt = (acpi_madt_t *)acpi_locate_table("APIC");
    g_lapic = (volatile uint32_t *)madt->local_apic_addr;
}

#endif