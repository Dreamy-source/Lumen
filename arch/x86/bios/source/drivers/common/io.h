#ifndef IO_H
#define IO_H

static inline void io_wait(void)
{
    outb(0x80, 0);
}

#endif