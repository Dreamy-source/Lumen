#ifndef HDA_MMIO_H
#define HDA_MMIO_H

#include "lib/baselib.h"

static inline void hda_enable_mmio(uint8_t dev, uint8_t func)
{
    uint32_t cmd = pci_read32(0, dev, func, 0x04);

    // bit 1 - Memory Space Enable
    if (!(cmd & 0x02))
    {
        pci_write32(0, dev, func, 0x04, cmd | 0x02);
    }
}

#endif