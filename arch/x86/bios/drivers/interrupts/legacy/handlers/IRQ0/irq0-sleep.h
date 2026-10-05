#ifndef IRQ0_SLEEP_H
#define IRQ0_SLEEP_H

#include "lib/baselib.h"
#include "interrupts/legacy/handlers/IRQ0/irq0-handler.h"

static inline void sleep(uint64_t ms)
{
    uint64_t target = timer_ticks + ms;
    while (timer_ticks < target) {
        __asm__ volatile ("hlt");
    }
}

#endif