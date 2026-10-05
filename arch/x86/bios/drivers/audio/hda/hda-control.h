#ifndef HDA_CONTROL_H
#define HDA_CONTROL_H

#include "lib/baselib.h"

static inline void hda_disable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    uint32_t gtcl = hda[2];

    // controller reset (crst = 0)
    gtcl &= ~0x1;
    hda[2] = gtcl;  // write
}

static inline void hda_enable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    uint32_t gtcl = hda[2];

    // controller exit reset state (crst = 1)
    gtcl &= ~0x0;
    hda[2] = gtcl;
}

static inline void hda_flush(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    // flush control = 1 (FCTRL, flush)
    uint32_t gtcl = hda[2];
    hda[2] = gtcl;

    while (!(*(volatile uint16_t*)((uint8_t*)hda + 0x10) & (1 << 10))){}
}

#endif