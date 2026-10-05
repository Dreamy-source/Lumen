#ifndef ACPI_RSDP_STRUCTINFO_H
#define ACPI_RSDP_STRUCTINFO_H

#include "lib/baselib.h"

typedef struct __attribute__((packed)) {
    char     Signature[8];
    uint8_t  Checksum;
    char     OEMID[6];
    uint8_t  Revision;
    uint32_t RSDT_Address;

    // ACPI 2.0+
    uint32_t Length;
    uint64_t XSDT_Address;
    uint8_t  ExtendedChecksum;
    uint8_t  Reserved[3];
} acpi_rsdp_t;

#endif