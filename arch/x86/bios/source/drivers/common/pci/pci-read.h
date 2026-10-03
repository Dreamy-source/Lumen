#ifndef PCI_READ_H
#define PCI_READ_H

#include "common/baselib.h"

#define ENABLE_BIT       0x80000000u
#define CONFIG_ADDRESS   0xCF8
#define CONFIG_DATA      0xCFC

static inline uint32_t pci_read32(uint8_t bus, uint8_t dev, uint8_t func, uint8_t offset)
{
    uint32_t address =
        ((uint32_t)bus    << 16)   |
        ((uint32_t)dev    << 11)   |
        ((uint32_t)func   << 8)    |
        ((uint32_t)offset & 0xFC)  |
        ((uint32_t)ENABLE_BIT);

        outl(CONFIG_ADDRESS, address);
        return inl(CONFIG_DATA);
}

#endif