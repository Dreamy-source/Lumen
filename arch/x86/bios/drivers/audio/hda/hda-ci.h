#ifndef HDA_CONTROLLER_INTERRUPTS_H
#define HDA_CONTROLLER_INTERRUPTS_H

#include "lib/baselib.h"

// Controller Interrupt Enable
static inline void hda_ci_enable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;

    hda[0x20/4] |= (1u << 30);
}

// Controller Interrupt Disable
static inline void hda_ci_disable(void)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    
    hda[0x20/4] &= ~(1u << 30);
}


#endif