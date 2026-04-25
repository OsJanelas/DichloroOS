#include "variables.h"
#include "ports.h"

#include "power.h"

void poweroff()
{
    outw(0x604, 0x2000);
    while (1) asm("hlt");
}

void restart()
{
    outb(0x64, 0xFE);
    while (1) asm("hlt");
}