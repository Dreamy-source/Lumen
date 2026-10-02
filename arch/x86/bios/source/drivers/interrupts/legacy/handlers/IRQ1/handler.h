#ifndef IRQ1_H
#define IRQ1_H

#define BACKSPACE_SCANCODE 0x0E
#define ENTER_SCANCODE 0x1C
#define RELEASE_BIT 0x80

#include "common/pci/pci.h"
#include "common/port.h"
#include "common/string.h"
#include "video/ascii/cp437.h"
#include "video/vga.h"
#include "interrupts/legacy/pic.h"
#include "interrupts/legacy/handlers/IRQ1/shell_parser.h"
#include "common/cpuid.h"
#include "audio/intel_hda.h"

static const char scancode_to_ascii[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t','q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0,   'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0,   '\\','z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0,   ' ',
};

static char buffer[VGA_WIDTH];
static uint8_t buffer_pos = 0;
static inline void buffer_reset(void)
{
    buffer[0] = '\0';
    buffer_pos = 0;
}

static inline void shell_print_from_buf(uint16_t print_from, uint16_t print_to, uint8_t color)
{
    for (int i = print_from; i < print_to; i++)
    {
        print_sym(buffer[i], color);
        buffer[i] = 0x00;
    }
    buffer_reset();
}

static inline void handle_cmd(const char* cmd, const char* execflag)
{
    if (streq("echo", cmd))
    {
        if (strneq(execflag, "-c", sizeofb("-c"))) 
        {
            uint8_t color;
            switch (buffer[8]) {
                case '0': color = 0x00; break;
                case '1': color = 0x01; break;
                case '2': color = 0x02; break;
                case '3': color = 0x03; break;
                case '4': color = 0x04; break;
                case '5': color = 0x05; break;
                case '6': color = 0x06; break;
                case '7': color = 0x07; break;
                case '8': color = 0x08; break;
                case '9': color = 0x09; break;
                case 'a': case 'A': color = 0x0A; break;
                case 'b': case 'B': color = 0x0B; break;
                case 'c': case 'C': color = 0x0C; break;
                case 'd': case 'D': color = 0x0D; break;
                case 'e': case 'E': color = 0x0E; break;
                case 'f': case 'F': color = 0x0F; break;
                default:
                    kprintf("lumen: %s: unknown echo color '%s'", 0x07, 0x0C, "error", color);
                    color = 0x00;
                    break;
            }
            shell_print_from_buf(10, VGA_WIDTH, color);
        }
        else
        {
            shell_print_from_buf(5, VGA_WIDTH, 0x07);
        }
    }
    else if (streq("clear", cmd))
    {
        clear();
    }
    else if (streq("cpuinfo", cmd))
    {
        uint16_t lines = 24;
        if (cpos >= VGA_HEIGHT)
        {
            scroll_screen_by(lines);
        }

        char vendor_p0[16];
        char vendor_p1[16];
        char vendor_p2[16];
        get_cpu_vendor_part0(vendor_p0);
        get_cpu_vendor_part1(vendor_p1);
        get_cpu_vendor_part2(vendor_p2);
        print_str("CPU Info:\n", 0x07);
        kprintf("  Vendor partition 1: %s\n", 0x07, 0x0F, vendor_p0);
        kprintf("  Vendor partition 2: %s\n", 0x07, 0x0F, vendor_p1);
        kprintf("  Vendor partition 3: %s\n", 0x07, 0x0F, vendor_p2);

        cpu_features_t cpu_features = get_cpu_features();

        print_str("CPU Features:\n", 0x07);
        kprintf("  FPU:       %s\n", 0x07, 0x0F, cpu_features.fpu     ? "yes" : "no");
        kprintf("  SSE:       %s\n", 0x07, 0x0F, cpu_features.sse     ? "yes" : "no");
        kprintf("  SSE2:      %s\n", 0x07, 0x0F, cpu_features.sse2    ? "yes" : "no");
        kprintf("  AVX:       %s\n", 0x07, 0x0F, cpu_features.avx     ? "yes" : "no");
        kprintf("  VMX:       %s\n", 0x07, 0x0F, cpu_features.vmx     ? "yes" : "no");
        kprintf("  MMX:       %s\n", 0x07, 0x0F, cpu_features.mmx     ? "yes" : "no");
        kprintf("  SSE3:      %s\n", 0x07, 0x0F, cpu_features.sse3    ? "yes" : "no");
        kprintf("  SSSE3:     %s\n", 0x07, 0x0F, cpu_features.ssse3   ? "yes" : "no");
        kprintf("  SSE4.1:    %s\n", 0x07, 0x0F, cpu_features.sse41   ? "yes" : "no");
        kprintf("  SSE4.2:    %s\n", 0x07, 0x0F, cpu_features.sse42   ? "yes" : "no");
        kprintf("  RDRAND:    %s\n", 0x07, 0x0F, cpu_features.rdrand  ? "yes" : "no");
        kprintf("  AES-NI:    %s\n", 0x07, 0x0F, cpu_features.aesni   ? "yes" : "no");
        kprintf("  SHA:       %s\n", 0x07, 0x0F, cpu_features.sha     ? "yes" : "no");
        print_str("CPU Modinfo:\n", 0x07);
        kprintf("  x86_64:    %s\n", 0x07, 0x0F, cpu_features.x86_64  ? "yes" : "no");
        kprintf("  NX:        %s\n", 0x07, 0x0F, cpu_features.nx      ? "yes" : "no");
        kprintf("  Syscall:   %s\n", 0x07, 0x0F, cpu_features.syscall ? "yes" : "no");
        kprintf("  1GB Pages: %s\n", 0x07, 0x0F, cpu_features.p1gb    ? "yes" : "no");
        kprintf("  2MB Pages: %s\n", 0x07, 0x0F, cpu_features.p2mb    ? "yes" : "no");
    }
    else if (streq("hdactl locate", cmd))
    {
        hda_locate();
        kprintf("[HDA] locate: devaddr=%h, dev=%h, func=%h, bar0=%h", 0x07, 0x0F, hda_devaddr, hda_t.dev, hda_t.func, hda_t.bar0);
    }
    else if (streq("hdactl enable mmio", cmd))
    {
        hda_enable_mmio(hda_t.dev, hda_t.func);
        kprintf("[HDA]: %s\n", 0x07, 0x0A, "ok");
    }
    else if (streq("hdactl start ini", cmd))
    {
        hda_ini();
    }
    else if (streq("mmap", cmd))
    {
        uint64_t cr3;
        asm volatile("mov %%cr3, %0" : "=r"(cr3));

        uint64_t *pml4 = (uint64_t*)(cr3 & ~0xFFF);
        uint64_t pml4e = pml4[0];
        kprintf("PML4[0]:   %h\n", 0x07, 0x0F, pml4e);

        uint64_t *pdpt = (uint64_t*)(pml4e & ~0xFFF);
        uint64_t pdpte = pdpt[3];
        kprintf("PDPT[3]:   %h\n", 0x07, 0x0F, pdpte);

        uint64_t *pd = (uint64_t*)(pdpte & ~0xFFF);
        uint64_t pde = pd[(0xFEBF0000ULL >> 21) & 0x1FF];
        kprintf("PD[0x1F5]: %h\n", 0x07, 0x0F, pde);
    }
    buffer_reset();
    return;
}

void irq1_handler_c(void)
{
    uint8_t scancode = inb(0x60);
    
    if (scancode & RELEASE_BIT){}
    else
    {
        if (scancode == BACKSPACE_SCANCODE)
        {
            if (cpos > fcpos)
            {
                if (buffer_pos > 0)
                {
                    buffer[buffer_pos--] = 0x00;
                    VGA[cpos * 2]     = ' ';
                    VGA[cpos * 2 + 1] = 0x07;
                    cpos--;
                    VGA[cpos * 2]     = ' ';
                    VGA[cpos * 2 + 1] = 0x07;
                    print_cursor(ASCII_BACK_TRIANGLE, 0x0F);
                }
            }
        }
        else if (scancode == ENTER_SCANCODE)
        {
            newline();
            
            // echo
            if (strneq(buffer, "echo", sizeofb("echo")))
            {
                handle_cmd("echo", "-c");
            }
            // clear
            else if (strneq(buffer, "clear", sizeofb("clear")))
            {
                handle_cmd("clear", "");
            }
            else if (strneq(buffer, "cpuinfo", sizeofb("cpuinfo")))
            {
                handle_cmd("cpuinfo", "");
            }
            else if (strneq(buffer, "hdactl locate", sizeofb("hdactl locate")))
            {
                handle_cmd("hdactl locate", "");
            }
            else if (strneq(buffer, "hdactl enable mmio", sizeofb("hdactl enable mmio")))
            {
                handle_cmd("hdactl enable mmio", "");
            }
            else if (strneq(buffer, "hdactl start ini", sizeofb("hdactl start ini")))
            {
                handle_cmd("hdactl start ini", "");
            }
            else if (strneq(buffer, "mmap", sizeofb("mmap")))
            {
                handle_cmd("mmap", "");
            }
            else
            {
                buffer_reset();
                kprintf("lumen: %s: unknown command handler", 0x07, 0x0C, "error");
            }

            newline();
            if (cpos >= VGA_WIDTH * VGA_HEIGHT)
            {
                scroll_screen();
            }
            print_shellrequest();
        }
        else
        {
            char c = scancode_to_ascii[scancode];
            print_sym(c, 0x0F);
            fcpos--;
            buffer[buffer_pos++] = c;
            print_cursor(ASCII_NEXT_TRIANGLE, 0x0F);
        }
    }
    pic_send_eoi(1);
}

#endif