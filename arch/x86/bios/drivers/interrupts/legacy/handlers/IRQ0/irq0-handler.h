#ifndef IRQ0_HANDLER_H
#define IRQ0_HANDLER_H

#include "lib/baselib.h"
#include "interrupts/legacy/pic/pic-send-eoi.h"

static volatile uint64_t pit_timer_ticks = 0;
void irq0_handler_c(void)
{
    pit_timer_ticks++;
    pic_send_eoi(0);
}

#endif