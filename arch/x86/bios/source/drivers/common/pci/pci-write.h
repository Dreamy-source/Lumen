#ifndef PCI_WRITE_H
#define PCI_WRITE_H

#define ENABLE_BIT       0x80000000u
#define CONFIG_ADDRESS   0xCF8
#define CONFIG_DATA      0xCFC

#include "common/baselib.h"

static inline void pci_write32(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset, uint32_t val)
{
    uint32_t address =
        ((uint32_t)bus    << 16)   |
        ((uint32_t)dev    << 11)   |
        ((uint32_t)func   << 8)    |
        ((uint32_t)offset & 0xFC)  |
        ((uint32_t)ENABLE_BIT);

        outl(CONFIG_ADDRESS, address);
        outl(CONFIG_DATA, val);
}

#endif