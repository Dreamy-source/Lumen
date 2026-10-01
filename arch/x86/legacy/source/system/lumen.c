#include "audio/intel_hda.h"
#include "common/liblumen.h" 

void lumen_main(void)
{
    bios_c_disable();
    clear();
    kprintf("Welcome to %s!\n", 0x0F, 0x0B, "Lumen");
    newline();

    pic_init(0x20, 0x28);

    idt_init();
    idt_set_descriptor(0x20, (void*)irq0_handler_asm, 0x8E);
    idt_set_descriptor(0x21, (void*)irq1_handler_asm, 0x8E);
    
    pit_init(1000);

    __asm__ volatile ("sti");

    hda_device_t hda_t;
    locate_hda();
    hda_enable_mmio(hda_t.dev, hda_t.func);

    print_shellrequest();

    while (1)
    {
        print_c('_', 0x0F);
        sleep(200);
        print_c(' ', 0x0F);
        sleep(200);
    }
}