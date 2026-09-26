#ifndef IDT_H
#define IDT_H

#include "../common/types.h"

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

#endif