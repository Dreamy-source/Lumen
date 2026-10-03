#ifndef IDT_HANDLERS_H
#define IDT_HANDLERS_H

#include "common/baselib.h"

extern void irq0_handler_asm(void);
extern void irq1_handler_asm(void);

static __attribute__((noinline)) void stub_handler(void)
{
    __asm__ volatile ("hlt");
}

#endif