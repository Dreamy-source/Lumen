#ifndef IRQ1_H
#define IRQ1_H

#include "common/port.h"
#include "common/string.h"
#include "video/vga.h"
#include "interrupts/pic.h"

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
            return;
        }
        else
        {
            shell_print_from_buf(5, VGA_WIDTH, 0x07);
            return;
        }
    }
    else if (streq("clear", cmd))
    {
        clear();
        buffer_reset();
        return;
    }
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
                buffer[buffer_pos--] = 0x00;
                VGA[cpos * 2]     = ' ';
                VGA[cpos * 2 + 1] = 0x07;
                cpos--;
                VGA[cpos * 2]     = ' ';
                VGA[cpos * 2 + 1] = 0x07;
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
            print_c('_', 0x0F);
        }
    }
    pic_send_eoi(1);
}

#endif