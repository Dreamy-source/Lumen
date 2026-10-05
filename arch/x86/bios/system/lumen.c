#include "acpi/rsdp/acpi-rsdp-locate.h"
#include "lib/liblumen.h"

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
    //hda_ini();
    newline();

    acpi_rsdp_t* rsdp_addr = acpi_rsdp_locate();
    if (rsdp_addr == NULL)
    {
        kprintf("[ACPI] RSDP not found\n", 0x07, 0x0F);
    }

    kprintf("[ACPI] RSDP Address:   %h\n", 0x07, 0x0F, rsdp_addr);
    kprintf("[ACPI] RSDP Signature: %s\n", 0x07, 0x0F, rsdp_addr->Signature);
    kprintf("[ACPI] RSDP OEMID:     %s\n", 0x07, 0x0F, rsdp_addr->OEMID);
    kprintf("[ACPI] RSDP Revision:  %d\n", 0x07, 0x0F, rsdp_addr->Revision);
    kprintf("[ACPI] RSDT Address:   %h\n", 0x07, 0x0F, rsdp_addr->RSDT_Address);

    print_shellrequest();

    while (1)
    {
        cursor_blink();
        wait_for_interrupt();
    }
}