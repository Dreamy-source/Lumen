#ifndef IDT_H
#define IDT_H

#include "common/types.h"

extern void irq0_handler_asm(void);
extern void irq1_handler_asm(void);

typedef struct {
    uint16_t Low;
    uint16_t Selector;
    uint8_t  IST;
    uint8_t  Type;
    uint16_t Mid;
    uint32_t High;
    uint32_t Zero;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t	Limit;
	uint64_t	Base;
} __attribute__((packed)) idtr_t;

__attribute__((aligned(0x10))) 
static idt_entry_t idt[256];

static idtr_t idtr;

static __attribute__((noinline)) void stub_handler(void)
{
    __asm__ volatile ("hlt");
}

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