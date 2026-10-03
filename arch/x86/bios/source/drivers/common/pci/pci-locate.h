#ifndef PCI_LOCATE_H
#define PCI_LOCATE_H

#define PCI_NOTFOUND     0xFFFFFFFF
#define PCI_DEV_NOTFOUND 0xFFFF

#include "common/baselib.h"
#include "common/pci/pci-read.h"

static inline uint32_t pci_locate_dev32(uint32_t prefdev, uint8_t bus)
{
    for (uint8_t dev = 0; dev < 32; dev++)
    {
        for (uint8_t func = 0; func < 8; func++)
        {
            uint32_t vendor_device = pci_read32(bus, dev, func, 0x00);
            uint16_t vendor = vendor_device & PCI_DEV_NOTFOUND;

            if (vendor == PCI_DEV_NOTFOUND) { continue; }

            if (vendor_device == prefdev)
            {
                return (dev << 8) | func;
            }
        }
    }
    return PCI_NOTFOUND;
}

#endif