#include "Qemu/debugcon_printf.h"


void outb(char ch, unsigned _iobase)
{
    __asm__ __volatile__("outb %%al, %%dx": : "a" (ch), "d" (_iobase));
}

void debug_putchar(char ch)
{
    outb(ch, (IOBASE));
}