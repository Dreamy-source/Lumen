#ifndef IRQ1_H
#define IRQ1_H

#define BACKSPACE_SCANCODE 0x0E
#define ENTER_SCANCODE 0x1C
#define RELEASE_BIT 0x80

#include "lib/baselib.h"

#include "interrupts/legacy/handlers/IRQ1/irq1-buffer.h"
#include "interrupts/legacy/handlers/IRQ1/irq1-handlecmd.h"
#include "interrupts/legacy/handlers/IRQ1/irq1-translate-tables.h"

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