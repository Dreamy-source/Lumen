#ifndef IRQ1_BUFFER_H
#define IRQ1_BUFFER_H

#include "common/baselib.h"

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

#endif