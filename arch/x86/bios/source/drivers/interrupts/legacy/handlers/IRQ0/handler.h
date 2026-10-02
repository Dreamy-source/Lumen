#ifndef IRQ0_H
#define IRQ0_H

#include "common/types.h"
#include "common/port.h"
#include "interrupts/legacy/pic.h"

static inline void pit_init(uint32_t hz)
{
    uint32_t divisor = 1193182 / hz;

    if (divisor > 0xFFFF) divisor = 0xFFFF;
    if (divisor < 1)      divisor = 1;

    // channel 0, rw=11, mode 3, binary
    outb(0x43, 0x36);

    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

static volatile uint64_t timer_ticks = 0;
void irq0_handler_c(void)
{
    timer_ticks++;
    pic_send_eoi(0);
}

static inline void sleep(uint64_t ms)
{
    uint64_t target = timer_ticks + ms;
    while (timer_ticks < target) {
        __asm__ volatile ("hlt");
    }
}

#endif