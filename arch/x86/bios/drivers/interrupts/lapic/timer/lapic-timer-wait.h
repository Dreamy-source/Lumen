#ifndef LAPIC_TIMER_WAIT_H
#define LAPIC_TIMER_WAIT_H

#include "lib/baselib.h"
#include "interrupts/lapic/timer/lapic-timer-handler.h"

// partial unstable
static inline void lapic_timer_wait_sec(uint64_t sec)
{
    uint64_t target = lapic_timer_ticks + sec * 10000;
    while (lapic_timer_ticks < target) {
        __asm__ volatile ("hlt");
    }
}

static inline void lapic_timer_wait_ms(uint64_t ms)
{
    uint64_t target = lapic_timer_ticks + ms * 10;
    while (lapic_timer_ticks < target) {
        __asm__ volatile ("hlt");
    }
}

#endif