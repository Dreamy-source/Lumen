#ifndef ACPI_LOCATE_T_H
#define ACPI_LOCATE_T_H

#include "lib/baselib.h"
#include "acpi/rsdp/acpi-rsdp-structinfo.h"
#include "acpi/acpi-header.h"
#include "acpi/rsdp/acpi-rsdp-locate.h"
#include "acpi/madt/acpi-madt-structinfo.h"

static inline acpi_header_t *acpi_locate_table(const char* table)
{
    acpi_rsdp_t *rsdp = acpi_rsdp_locate();
    if (rsdp == NULL) return NULL;

    acpi_rsdt_t *rsdt = (acpi_rsdt_t *)rsdp->RSDT_Address;
    if (rsdt == NULL) return NULL;

    if (!strneq(rsdt->header.Signature, "RSDT", 4)) return NULL;

    uint32_t t_count = (rsdt->header.Length - sizeof(acpi_header_t)) / 4;

    for (uint32_t i = 0; i < t_count; i++) {
        acpi_header_t *t = (acpi_header_t *)rsdt->entries[i];
        if (strneq(t->Signature, table, 4)) {
            return t;
        }
    }
    return NULL;
}

#endif