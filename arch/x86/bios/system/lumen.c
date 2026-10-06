#include "lib/liblumen.h"

static hda_device_t hda_t;
void lumen_main(void)
{
    vga_cursor_disable();
    clear();
    kprintf("Welcome to %s!\n", 0x0F, 0x0B, "Lumen");
    newline();

    // PIC initialization
    pic_init(0x20, 0x28);
    pic_mask(0xFF, 0xFF);

    // IDT initialization
    idt_init();
    idt_set_descriptor(0x20, (void*)lapic_timer_handler_asm, 0x8E);
    idt_set_descriptor(0x21, (void*)irq1_handler_asm, 0x8E);

    // LAPIC initialization
    lapic_enable();
    lapic_init();
    lapic_wrtpr(LAPIC_TPR_ALLOW_ALL_PRIORITY_INTERRUPTS);
    lapic_wreoi(LAPIC_EOI_RESET_ALL_OLD_INTERRUPTS);
    lapic_wrsvr(LAPIC_SVR_ENABLE | LAPIC_SVR_SPURIOUS_VEC);
    lapic_wresr(LAPIC_ESR_CLEAR_ALL_ERROR_REGISTER, LAPIC_ESR_CLEAR_ALL_ERROR_REGISTER);

    // LAPIC Timer initialization
    lapic_timer_init();

    enable_interrupts();
    
    // by default, lumen finds the HDA device on the PCI bus automatically
    hda_locate();
    hda_enable_mmio(hda_t.dev, hda_t.func);
    hda_ini();
    newline();

    // ACPI initialization
    acpi_rsdp_t* rsdp = acpi_rsdp_locate();
    if (rsdp == NULL)
    {
        kprintf("[ACPI] RSDP not found\n", 0x07, 0x0F);
    }

    acpi_madt_t* madt = (acpi_madt_t*)acpi_locate_table("APIC");

    kprintf("[ACPI] MADT LAPIC Address: %h\n", 0x07, 0x0F, madt->local_apic_addr);

    uint32_t id = g_lapic[0x20 / 4];
    kprintf("[APIC] LAPIC ID: %d\n", 0x07, 0x0F, id >> 24);
    lapic_timer_init();

    print_shellrequest();

    while (1)
    {
        cursor_blink();
        wait_for_interrupt();
    }
}