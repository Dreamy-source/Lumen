#ifndef ACPI_HEADER_H
#define ACPI_HEADER_H

#include "lib/baselib.h"

typedef struct __attribute__((packed)) {
    char     Signature[4];
    uint32_t Length;
    uint8_t  Revision;
    uint8_t  Checksum;
    char     OEMID[6];
    char     OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;
} acpi_header_t;

typedef struct {
    acpi_header_t header;
    uint32_t      entries[];
} acpi_rsdt_t;

#endif