#ifndef PIC_SEND_EOI_H
#define PIC_SEND_EOI_H

#include "lib/baselib.h"

static inline void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8) outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

#endif