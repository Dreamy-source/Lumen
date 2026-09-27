#include "common/liblumen.h"
#include "video/vga.h"
#include "interrupts/pic.h"
#include "interrupts/idt.h"
#include "interrupts/handlers/0.h"
#include "interrupts/handlers/1.h"
#include "audio/intel_hda.h"
#include "audio/pc_speaker.h"
#include "common/pci/pci.h"
#include "common/pci/express/pci_express.h"

void lumen_main(void)
{
    bios_c_disable();
    clear();
    print_str("Welcome to ", 0x0F);
    print_str("Lumen", 0x0B);
    print_sym('!', 0x0F);
    newline();

    pic_init(0x20, 0x28);

    idt_init();
    idt_set_descriptor(0x20, (void*)irq0_handler_asm, 0x8E);
    idt_set_descriptor(0x21, (void*)irq1_handler_asm, 0x8E);
    
    pit_init(1000);

    __asm__ volatile ("sti");

    newline();
    print_shellrequest();

    while (1)
    {
        print_c('_', 0x07);
        sleep(200);
        print_c(' ', 0x07);
        sleep(200);
    }
}