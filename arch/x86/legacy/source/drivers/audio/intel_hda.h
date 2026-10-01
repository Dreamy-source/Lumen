#ifndef HDA_H
#define HDA_H

#define HDA_ID 0x28868086

#include "common/types.h"
#include "common/pci.h"
#include "video/vga.h"

static uint32_t hda_devaddr;
static inline void locate_hda()
{
    hda_devaddr = pci_locate_dev32(HDA_ID, 0);    
}

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