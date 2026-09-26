#include "../drivers/video/vga.h"

void lumen_main(void)
{
    clear();
    print_str("Welcome to ", 0x0F);
    print_str("Lumen", 0x0B);
    print_sym('!', 0x0F);

    while (1)
    {
        print_c('_', 0x0F);
    }
}