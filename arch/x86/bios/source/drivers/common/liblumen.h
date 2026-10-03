#ifndef LIB_LUMEN_H
#define LIB_LUMEN_H

// base
#include "common/types.h"
#include "common/port.h"
#include "common/io.h"
#include "common/string.h"
#include "common/msr.h"

// cpuid
#include "common/cpuid/cpuid.h"
#include "common/cpuid/cpuid-vendor.h"
#include "common/cpuid/cpuid-features.h"
#include "common/cpuid/cpuid-features-structinfo.h"

// pci
    // pci
    #include "common/pci/pci-locate.h"
    #include "common/pci/pci-exists.h"
    #include "common/pci/pci-read.h"
    #include "common/pci/pci-write.h"

    // pci-e
    #include "common/pci/express/pci-e.h"

// video
#include "video/legacy/vga.h"

// interrupts

    // pic
    #include "interrupts/legacy/pic/pic-init.h"
    #include "interrupts/legacy/pic/pic-send-eoi.h"

    // idt
    #include "interrupts/legacy/idt/idt-init.h"
    #include "interrupts/legacy/idt/idt-set-descriptor.h"
    #include "interrupts/legacy/idt/idt-handlers.h"
    #include "interrupts/legacy/idt/idt-interrupts.h"
    #include "interrupts/legacy/idt/idt-structinfo.h"

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

#endif