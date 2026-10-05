#ifndef ACPI_RSDP_LOCATE_H
#define ACPI_RSDP_LOCATE_H

#include "lib/baselib.h"
#include "drivers/acpi/rsdp/acpi-rsdp-structinfo.h"

static inline acpi_rsdp_t* acpi_rsdp_locate(void)
{
    for (uintptr_t i = 0x000E0000; i < 0x000FFFFF; i += 16)
    {
        if (strneq("RSD PTR ", (const char*)i, sizeofb("RSD PTR ")))
        {
            return (acpi_rsdp_t*)i;
        }
    }

    return false;
}

#endif