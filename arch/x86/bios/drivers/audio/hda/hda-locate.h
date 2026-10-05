#ifndef HDA_LOCATE_H
#define HDA_LOCATE_H

#define HDA_ID       0x26688086

#include "lib/baselib.h"
#include "audio/hda/hda-structinfo.h"
#include "bus/pci/pci-read.h"
#include "bus/pci/pci-locate.h"

static uint32_t hda_devaddr;
static inline void hda_locate(void)
{
    hda_devaddr = pci_locate_dev32(HDA_ID, 0);

    if (hda_devaddr == PCI_NOTFOUND)
    {
        kprintf("[HDA] %s\n", 0x07, 0x0C, "not found");
        hda_t.dev = 0xFF;
        hda_t.func = 0xFF;
        return;
    }

    hda_t.dev = (hda_devaddr >> 8) & 0xFF;
    hda_t.func = hda_devaddr & 0xFF;
    hda_t.bar0 = pci_read32(0, hda_t.dev, hda_t.func, 0x10);
}

#endif