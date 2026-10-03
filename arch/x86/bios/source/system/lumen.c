#include "common/liblumen.h"

static hda_device_t hda_t;
void lumen_main(void)
{
    vga_cursor_disable();
    clear();
    kprintf("Welcome to %s!\n", 0x0F, 0x0B, "Lumen");
    newline();

    pic_init(0x20, 0x28);

    idt_init();
    idt_set_descriptor(0x20, (void*)irq0_handler_asm, 0x8E);
    idt_set_descriptor(0x21, (void*)irq1_handler_asm, 0x8E);
    
    pit_init(1000);

    enable_interrupts();
    
    // by default, lumen finds the HDA device on the PCI bus automatically
    hda_locate();
    hda_enable_mmio(hda_t.dev, hda_t.func);
    hda_ini();

    print_shellrequest();

    while (1)
    {
        cursor_blink();
        wait_for_interrupt();
    }
}