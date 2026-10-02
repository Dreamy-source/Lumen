#ifndef IRQ1_H
#define IRQ1_H

#include "common/port.h"
#include "common/string.h"
#include "video/ascii/cp437.h"
#include "video/vga.h"
#include "interrupts/legacy/pic.h"
#include "interrupts/legacy/handlers/IRQ1/shell_parser.h"
#include "common/cpuid.h"

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
        if (strneq(execflag, "-c", count_of_bytes("-c"))) 
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
                    print_str("lumen: error: unknown color", 0x0C);
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
    buffer_reset();
    return;
}

void irq1_handler_c(void)
{
    uint8_t scancode = inb(0x60);
    
    if (scancode & 0x80){}
    else
    {
        if (scancode == 0x0E)
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
        else if (scancode == 0x1C)
        {
            newline();
            
            // echo
            if (strneq(buffer, "echo", count_of_bytes("echo")))
            {
                handle_cmd("echo", "-c");
            }
            // clear
            else if (strneq(buffer, "clear", count_of_bytes("clear")))
            {
                handle_cmd("clear", "");
            }
            else if (strneq(buffer, "cpuinfo", count_of_bytes("cpuinfo")))
            {
                handle_cmd("cpuinfo", "");
            }
            else
            {
                buffer_reset();
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