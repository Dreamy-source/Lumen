#ifndef ACPI_MADT_STRUCTINFO_H
#define ACPI_MADT_STRUCTINFO_H

#include "lib/baselib.h"
#include "acpi/acpi-header.h"

typedef struct __attribute__((packed)) {
    acpi_header_t header;
    uint32_t      local_apic_addr;  // LAPIC address
    uint32_t      flags;            // bit 0 = PCAT_COMPAT
    uint8_t       entries[];
} acpi_madt_t;

#endif