#ifndef IDT_STRUCTINFO_H
#define IDT_STRUCTINFO_H

#include "common/baselib.h"

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

#endif