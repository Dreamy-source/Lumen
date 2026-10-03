#ifndef IRQ0_INIT_H
#define IRQ0_INIT_H

#include "common/baselib.h"

static inline void pit_init(uint32_t hz)
{
    uint32_t divisor = 1193182 / hz;

    if (divisor > 0xFFFF) divisor = 0xFFFF;
    if (divisor < 1)      divisor = 1;

    // channel 0, rw=11, mode 3, binary
    outb(0x43, 0x36);

    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

#endif