#ifndef PIC_INIT_H
#define PIC_INIT_H

#include "lib/baselib.h"
#include "interrupts/legacy/pic/pic-mask.h"

static inline void pic_init(uint16_t irq0_8_offset, uint16_t irq8_15_offset)
{
    outb(0x20, 0x10 | 0x01);    io_wait();
    outb(0xA0, 0x10 | 0x01);    io_wait();

    outb(0x21, irq0_8_offset);  io_wait();
    outb(0xA1, irq8_15_offset); io_wait();

    outb(0x21, 1 << 2);         io_wait();
    outb(0xA1, 2);              io_wait();

    outb(0x21, 0x01);           io_wait();
    outb(0xA1, 0x01);           io_wait();

    pic_mask(0xFC, 0xFF);
}

#endif