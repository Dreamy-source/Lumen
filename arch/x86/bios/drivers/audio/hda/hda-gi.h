#ifndef HDA_GLOBAL_INTERRUPTS_H
#define HDA_GLOBAL_INTERRUPTS_H

#include "lib/baselib.h"

// Global Interrupt Enable
static inline void hda_gi_enable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    // 0x20 - INTCTL (Interrupt Control)
    hda[0x20/4] |= (1u << 31);
}

// Global Interrupt Disable
static inline void hda_gi_disable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    hda[0x20/4] &= ~(1u << 31);
}

#endif