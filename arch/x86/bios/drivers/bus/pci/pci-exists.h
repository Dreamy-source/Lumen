#ifndef PCI_EXISTS_H
#define PCI_EXISTS_H

#define PCI_NOTFOUND 0xFFFFFFFF

#include "lib/baselib.h"
#include "bus/pci/pci-read.h"

static inline uint32_t pci_device_exists32(uint8_t bus, uint8_t dev)
{
    return pci_read32(bus, dev, 0, 0) != PCI_NOTFOUND;
}

#endif