#ifndef ISR_HANDLER_H
#define ISR_HANDLER_H

#include "../../common/types.h"
#include "../../video/vga.h"

static const char* failureDescriptions[32] = {
    "| [#DE] division by zero",
    "| [#DB] debug exception",
    "| [NMI] nonmaskable interrupt",
    "| [#BP] breakpoint",
    "| [#OF] overflow",
    "| [#BR] bound range",
    "| [#UD] invalid opcode",
    "| [#NM] device not available",
    "| [#DF] double fault",
    "| [#MP] coproc. segment overrun",
    "| [#TS] invalid tss",
    "| [#NP] segment not present",
    "| [#SS] stack-segment fault",
    "| [#GP] general protection fault",
    "| [#PF] page fault",
    "| [???] reserved",
    "| [#MF] x87 fp error",
    "| [#AC] alignment check",
    "| [#MC] machine check",
    "| [#XM] simd fp error",
    "| [#VE] virtualization",
    "| [#CP] control protection",
    "| [???] reserved",
    "| [???] reserved",
    "| [???] reserved",
    "| [???] reserved",
    "| [???] reserved",
    "| [???] reserved",
    "| [HV] hypervisor injection",
    "| [VC] vmm communication",
    "| [SX] security exception",
    "| [???] reserved",
};

static inline void isr_handler(uint8_t vector)
{
    if (vector < 32) {
        fill(' ', 0x0C);
        print_str("KERNEL PANIC", 0x0F);
        print_str(failureDescriptions[vector], 0x0F);
        for(;;) __asm__ volatile ("hlt");
    }
}

#endif