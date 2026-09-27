#ifndef VGA_H
#define VGA_H

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_SCREEN VGA_WIDTH * VGA_HEIGHT

#include "common/types.h"
#include "common/port.h"

static int cpos = 0;
static int fcpos = 0;
static volatile uint8_t* VGA = (volatile uint8_t*)VGA_MEMORY;
static inline void scroll_screen(void)
{
    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++)
    {
        VGA[i * 2]     = VGA[(i + 80) * 2];
        VGA[i * 2 + 1] = VGA[(i + 80) * 2 + 1];
    }
    for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_SCREEN; i++)
    {
        VGA[i * 2] = ' ';
        VGA[i * 2 + 1] = 0x00;
    }
    cpos = VGA_WIDTH * (VGA_HEIGHT - 1);
    fcpos = cpos;
}

static inline void newline()
{
    cpos += VGA_WIDTH - (cpos % VGA_WIDTH);
    fcpos = cpos;
}

static inline void print_sym(uint8_t sym, uint8_t color)
{
    if (sym == '\n')
    {
        newline();
        return;
    }
    VGA[cpos * 2] = sym;
    VGA[cpos * 2 + 1] = color; 
    cpos++;
    fcpos++;
}

static inline void print_str(const char* str, uint8_t color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        print_sym(str[i], color);
    }
}

static inline void print_hex(uint64_t v, int digits, uint8_t color)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    print_str("0x", color);
    for (int i = digits - 1; i >= 0; i--)
    {
        print_sym(hex_digits[(v >> (i * 4)) & 0xF], color);
    }
}

static inline void fill(uint8_t sym, uint8_t color)
{
    for (int i = 0; i < VGA_SCREEN; i++)
    {
        print_sym(sym, color);
    }
    cpos = 0;
    fcpos = cpos;
}

static inline void clear(void)
{
    for (int i = 17; i < VGA_SCREEN; i++)
    {
        VGA[i * 2] = ' ';
        VGA[i * 2 + 1] = 0x00;
    }
    cpos = 0;
    fcpos = cpos;
}

static inline void clear_all(void)
{
    for (int i = 0; i < VGA_SCREEN; i++)
    {
        VGA[i * 2] = ' ';
        VGA[i * 2 + 1] = 0x00;
    }
    cpos = 0;
    fcpos = cpos;
}

static inline void print_c(uint8_t sym, uint8_t color)
{
    VGA[cpos * 2] = sym;
    VGA[cpos * 2 + 1] = color;
}

static inline void bios_c_disable()
{
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static inline void print_shellrequest()
{
    print_str("[lumen] ", 0x0F);
}

#endif