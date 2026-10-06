#ifndef VGA_H
#define VGA_H

#define VGA_MEMORY 0xB8000
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_SCREEN VGA_WIDTH * VGA_HEIGHT

#include <stdarg.h>
#include "lib/baselib.h"

#include "interrupts/legacy/handlers/IRQ0/irq0-sleep.h"

#include "interrupts/lapic/timer/lapic-timer-wait.h"

#include "video/legacy/ascii/cp437.h"

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

static inline void clear(void)
{
    for (int i = 0; i < VGA_SCREEN; i++)
    {
        VGA[i * 2] = ' ';
        VGA[i * 2 + 1] = 0x00;
    }
    cpos = 0;
    fcpos = cpos;
}

static inline void newline(void)
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

static inline void print_dec(uint64_t n, uint8_t attr)
{
    static const char dec_digits[] = "0123456789";

    if (n == 0)
    {
        print_sym('0', attr);
        return;
    }

    if (n < 0)
    {
        print_sym('-', attr);
        n = -n;
    }

    char buf[20];
    int i = 0;

    while (n > 0)
    {
        buf[i++] = dec_digits[n % 10];
        n /= 10;
    }

    while (i > 0)
    {
        print_sym(buf[--i], attr);
    }
}

static inline void kprintf(const char* fmt, uint8_t attr, int spattr, ...)
{
    va_list args;
    va_start(args, spattr);

    for (const char* p = fmt; *p; p++)
    {
        if (*p != '%')
        {
            print_sym(*p, attr);
            continue;
        }

        p++;

        switch (*p)
        {
            case 'h':
            {
                unsigned int n = va_arg(args, unsigned int);
                print_hex(n, 8, spattr);
                break;
            }
            case 'd':
            {
                int n = va_arg(args, unsigned int);
                print_dec(n, spattr);
                break;
            }
            case 's':
            {
                const char* s = va_arg(args, const char*);
                print_str(s, spattr);
                break;
            }
            case 'c':
            {
                char c = (char)va_arg(args, int);
                print_sym(c, spattr);
                break;
            }
            case '%':
            {
                print_sym('%', attr);
                break;
            }
            default:
            {
                print_sym('%', attr);
                print_sym(*p, attr);
                break;
            }
        }
    }

    va_end(args);
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

static inline void print_cursor(uint8_t sym, uint8_t color)
{
    VGA[cpos * 2] = sym;
    VGA[cpos * 2 + 1] = color;
}

static inline void vga_cursor_disable()
{
    outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static inline void print_shellrequest()
{
    print_str("[lumen] ", 0x0F);
}

static inline void cursor_blink()
{
    print_cursor('_', 0x0F);
    lapic_timer_wait_ms(200);
    print_cursor(' ', 0x0F);
    lapic_timer_wait_ms(200);
}

static inline void scroll_screen_by(uint16_t pos)
{
    if (pos == 0 || pos >= VGA_HEIGHT)
    {
        clear();
        return;
    }

    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - pos); i++)
    {
        VGA[i * 2]     = VGA[(i + 80) * 2];
        VGA[i * 2 + 1] = VGA[(i + 80) * 2 + 1];
    }
    for (int i = VGA_WIDTH * (VGA_HEIGHT - pos); i < VGA_SCREEN; i++)
    {
        VGA[i * 2] = ' ';
        VGA[i * 2 + 1] = 0x00;
    }
    cpos = VGA_WIDTH * (VGA_HEIGHT - pos);
    fcpos = cpos;
}

#endif