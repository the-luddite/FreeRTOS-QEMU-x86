#include "Qemu/debugcon_printf.h"


void outb(char ch, unsigned _iobase)
{
    __asm__ __volatile__("outb %%al, %%dx": : "a" (ch), "d" (_iobase));
}

void debug_putchar(char ch)
{
    outb(ch, (IOBASE));
}

void qemu_printchar(char **str, int c)
{
	if (str) {
		**str = (char)c;
		++(*str);
	}
	else
	{
		debug_putchar( c );
	}
}