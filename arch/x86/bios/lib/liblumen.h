#ifndef LIB_LUMEN_H
#define LIB_LUMEN_H

// default
#include "default/types.h"

// cpu
#include "cpu/port.h"
#include "cpu/io.h"
#include "cpu/msr.h"
#include "cpu/cpuid/cpuid.h"
#include "cpu/cpuid/cpuid-vendor.h"
#include "cpu/cpuid/cpuid-features.h"
#include "cpu/cpuid/cpuid-features-structinfo.h"
#include "cpu/tsc/tsc-rdtsc.h"
#include "cpu/tsc/tsc-supported.h"

// string
#include "string/string-streq.h"
#include "string/string-sizeofb.h"
#include "string/string-sc-in-s.h"
#include "string/string-mem.h"
#include "string/string-splitwhitespace.h"
#include "string/string-strcat.h"
#include "string/string-strcp.h"

// pci
    // pci
    #include "bus/pci/pci-locate.h"
    #include "bus/pci/pci-exists.h"
    #include "bus/pci/pci-read.h"
    #include "bus/pci/pci-write.h"

    // pci-e
    #include "bus/pci-e/pci-e.h"

// video
#include "video/legacy/vga.h"

// interrupts

    // pic
    #include "interrupts/legacy/pic/pic-init.h"
    #include "interrupts/legacy/pic/pic-send-eoi.h"
    #include "interrupts/legacy/pic/pic-mask.h"
    
    // idt
    #include "interrupts/idt/idt-init.h"
    #include "interrupts/idt/idt-set-descriptor.h"
    #include "interrupts/idt/idt-handlers.h"
    #include "interrupts/idt/idt-interrupts.h"
    #include "interrupts/idt/idt-structinfo.h"

    // isr
    #include "interrupts/legacy/handlers/ISR/handler.h"

    // irq0
    #include "interrupts/legacy/handlers/IRQ0/irq0-handler.h"
    #include "interrupts/legacy/handlers/IRQ0/irq0-init.h"
    #include "interrupts/legacy/handlers/IRQ0/irq0-sleep.h"

    // irq1
    #include "interrupts/legacy/handlers/IRQ1/irq1-handler.h"
    #include "interrupts/legacy/handlers/IRQ1/irq1-handlecmd.h"
    #include "interrupts/legacy/handlers/IRQ1/irq1-buffer.h"
    #include "interrupts/legacy/handlers/IRQ1/irq1-translate-tables.h"

    // lapic
    #include "interrupts/lapic/lapic-init.h"
    #include "interrupts/lapic/lapic-enable.h"
    #include "interrupts/lapic/lapic-tpr.h"
    #include "interrupts/lapic/lapic-eoi.h"
    #include "interrupts/lapic/lapic-svr.h"
    #include "interrupts/lapic/lapic-esr.h"
    #include "interrupts/lapic/timer/lapic-timer-init.h"
    #include "interrupts/lapic/timer/lapic-timer-handler.h"

// audio
    // hda (high definition audio)
    #include "audio/hda/hda-structinfo.h"
    #include "audio/hda/hda-init.h"
    #include "audio/hda/hda-locate.h"
    #include "audio/hda/hda-mmio.h"
    #include "audio/hda/hda-requests.h"
    #include "audio/hda/hda-control.h"
    #include "audio/hda/hda-gi.h"
    #include "audio/hda/hda-ci.h"
    #include "audio/hda/hda-si.h"

    // pcspk (pc speaker)
    #include "audio/pcspk/pcspk.h"

// acpi
#include "drivers/acpi/acpi-header.h"
#include "drivers/acpi/acpi-locate-all_t.h"
#include "drivers/acpi/acpi-locate-t.h"

    // rsdp
    #include "drivers/acpi/rsdp/acpi-rsdp-locate.h"
    #include "drivers/acpi/rsdp/acpi-rsdp-structinfo.h"

    // madt
    #include "drivers/acpi/madt/acpi-madt-structinfo.h"

#endif