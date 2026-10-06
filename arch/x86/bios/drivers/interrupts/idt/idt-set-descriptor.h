#ifndef IDT_SET_DESCRIPTOR_H
#define IDT_SET_DESCRIPTOR_H

#include "lib/baselib.h"
#include "interrupts/idt/idt-structinfo.h"

static inline void idt_set_descriptor(uint8_t vector, void* isr, uint8_t flags)
{
    idt_entry_t* descriptor = &idt[vector];

    descriptor->Low            = (uint64_t)isr & 0xFFFF;
    descriptor->Selector       = 0x08;
    descriptor->IST            = 0;
    descriptor->Type           = flags;
    descriptor->Mid            = ((uint64_t)isr >> 16) & 0xFFFF;
    descriptor->High           = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
    descriptor->Zero           = 0;
}

#endif