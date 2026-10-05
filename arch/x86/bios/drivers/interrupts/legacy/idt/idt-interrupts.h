#ifndef IDT_INTERRUPTS_H
#define IDT_INTERRUPTS_H

#include "lib/baselib.h"

static inline void disable_interrupts(void)
{
    __asm__ volatile ("cli");
}

static inline void enable_interrupts(void)
{
    __asm__ volatile ("sti");
}

static inline void wait_for_interrupt(void)
{
    __asm__ volatile ("hlt");
}

#endif