#include "variables.h"
#include "text.h"
#include "ports.h"
#include "date.h"

USHORT getcmosmem()
{
    outb(0x70, 0x30);
    BYTE low = inb(0x71);

    outb(0x70, 0x31);
    BYTE high = inb(0x71);

    USHORT total = low | high << 8;

    return total;
}

void getcmosdate()
{
    outb(0x70, 0x0A);
    while(inb(0x71) & 0x80);

    outb(0x70, 0x07);
    BYTE day = inb(0x71);

    outb(0x70, 0x08);
    BYTE month = inb(0x71);

    outb(0x70, 0x09);
    BYTE year = inb(0x71);

    day   = ((day   / 16) * 10) + (day   & 0x0F);
    month = ((month / 16) * 10) + (month & 0x0F);
    year  = ((year  / 16) * 10) + (year  & 0x0F);

    printint(month, 0xFFFFFFFF);
    Print("/", 0xFFFFFFFF);

    printint(day, 0xFFFFFFFF);
    Print("/", 0xFFFFFFFF);

    printint(year, 0xFFFFFFFF);
}