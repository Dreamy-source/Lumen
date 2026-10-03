#ifndef HDA_STREAM_INTERRUPTS_H
#define HDA_STREAM_INTERRUPTS_H

#include "common/baselib.h"

// Stream Interrupt Enable
static inline void hda_si_enable(uint32_t stream)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    
    hda[0x20/4] |= (1u << stream);
}

// Stream Interrupt Disable
static inline void hda_si_disable(uint32_t stream)
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    
    hda[0x20/4] &= ~(1u << stream);
}

// Stream Interrupt Enable All
static inline void hda_si_enable_all()
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    
    hda[0x20/4] |= 0x3FFFFFFF;
}

// Stream Interrupt Disable All
static inline void hda_si_disable_all()
{
    volatile uint32_t* hda = (volatile uint32_t*)hda_t.bar0;
    
    hda[0x20/4] &= ~0x3FFFFFFF;
}

#endif