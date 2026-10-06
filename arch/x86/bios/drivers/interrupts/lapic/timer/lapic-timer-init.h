#ifndef LAPIC_TIMER_INIT_H
#define LAPIC_TIMER_INIT_H

#define LAPIC_TIMER_DIV  0x3E0
#define LAPIC_LVT_TIMER  0x320
#define LAPIC_TIMER_INIT 0x380
#define TIMER_VECTOR     0x20
#define TIMER_PERIODIC   (1 << 17)

#include "lib/baselib.h"
#include "interrupts/lapic/lapic-init.h"

static inline void lapic_timer_init()
{
    g_lapic[LAPIC_TIMER_DIV  / 4] = 0x03;
    g_lapic[LAPIC_LVT_TIMER  / 4] = TIMER_VECTOR | TIMER_PERIODIC;
    g_lapic[LAPIC_TIMER_INIT / 4] = 6250;
}

#endif