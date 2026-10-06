#ifndef LAPIC_TIMER_HANDLER_H
#define LAPIC_TIMER_HANDLER_H

#include "lib/baselib.h"
#include "interrupts/lapic/lapic-eoi.h"

static uint64_t lapic_timer_ticks;
void lapic_timer_handler_c(void)
{
    lapic_timer_ticks++;
    lapic_wreoi(0);
}

#endif