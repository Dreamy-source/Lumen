#ifndef IRQ1_HANDLECMD_H
#define IRQ1_HANDLECMD_H

#include "lib/baselib.h"

#include "interrupts/legacy/handlers/IRQ1/irq1-buffer.h"

#include "cpu/cpuid/cpuid-vendor.h"
#include "cpu/cpuid/cpuid-features.h"

#include "audio/hda/hda-init.h"
#include "audio/hda/hda-locate.h"
#include "audio/hda/hda-mmio.h"

static inline void handle_cmd(const char* cmd, const char* execflag)
{
    if (streq("echo", cmd))
    {
        if (strneq(execflag, "-c", sizeofb("-c"))) 
        {
            uint8_t color = 0x07;
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

#endif