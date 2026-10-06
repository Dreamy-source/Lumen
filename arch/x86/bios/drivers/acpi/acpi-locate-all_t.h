#ifndef ACPI_ALL_T_LOCATE_H
#define ACPI_ALL_T_LOCATE_H

#include "lib/baselib.h"
#include "acpi/acpi-header.h"
#include "acpi/rsdp/acpi-rsdp-locate.h"

static inline uint8_t acpi_all_locate()
{
    acpi_rsdp_t* rsdp = acpi_rsdp_locate();
    acpi_rsdt_t* rsdt = (acpi_rsdt_t*)rsdp->RSDT_Address;
    uint32_t t_count = (rsdt->header.Length - sizeof(acpi_header_t)) / 4;

    for (uint32_t i = 0; i < t_count; i++)
    {
        uint32_t addr = rsdt->entries[i];
        acpi_header_t* t = (acpi_header_t*)rsdt->entries[i];

        kprintf("[ACPI] Found: ", 0x07, 0x0F);
        for (int j = 0; j < 4; j++) {
            kprintf("%c", 0x07, 0x0F, t->Signature[j]);
        }
        kprintf(" (addr %h)", 0x07, 0x0F, addr);
        newline();
    }
    if (rsdp == NULL) return 0;
}

#endif