#ifndef IDT_INIT_H
#define IDT_INIT_H

#include "default/types.h"
#include "interrupts/idt/idt-set-descriptor.h"
#include "interrupts/idt/idt-handlers.h"

static inline void idt_init(void)
{
    idtr.Base = (uintptr_t)&idt[0];
    idtr.Limit = sizeof(idt) - 1;

    for (int i = 0; i < 256; i++) {
        idt_set_descriptor(i, (void*)stub_handler, 0x8E);
    }

    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

#endif