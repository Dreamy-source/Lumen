#ifndef VGA_H
#define VGA_H

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

#include "../common/types.h"

static int cpos = 0;
static int fcpos = 0;
static volatile uint8_t* VGA = (volatile uint8_t*)VGA_MEMORY;
static inline void print_sym(uint8_t sym, uint8_t color)
{
    if (sym == '\n')
    {
        cpos += 80 - (cpos % 80);
        fcpos = cpos;
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

static inline void fill(uint8_t sym, uint8_t color)
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        print_sym(sym, color);
    }
    cpos = 0;
    fcpos = 0;
}

static inline void clear()
{
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
    {
        print_sym(' ', 0x00);
    }
    cpos = 0;
    fcpos = 0;
}

static inline void print_c(uint8_t sym, uint8_t color)
{
    VGA[fcpos * 2] = sym;
    VGA[fcpos * 2 + 1] = color;
}

#endif