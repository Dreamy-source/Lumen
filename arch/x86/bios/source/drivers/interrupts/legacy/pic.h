#ifndef PIC_H
#define PIC_H

#include "common/port.h"
#include "common/io.h"

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

    outb(0x21, 0b11111100);     io_wait();
    outb(0xA1, 0b11111111);     io_wait();
}

static inline void pic_send_eoi(uint8_t irq)
{
    if (irq >= 8) outb(0xA0, 0x20);
    outb(0x20, 0x20);
}

#endif