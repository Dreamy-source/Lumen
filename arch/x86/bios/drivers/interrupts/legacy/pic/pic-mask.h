#ifndef PIC_MASK_H
#define PIC_MASK_H

#include "lib/baselib.h"

static inline void pic_mask(uint8_t master, uint8_t slave)
{
    outb(0x21, master); io_wait();
    outb(0xA1, slave);  io_wait();
}

#endif