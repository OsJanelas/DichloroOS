#include "variables.h"
#include "video.h"
#include "text.h"
#include "idt.h"
#include "ports.h"
#include "graphics.h"

#include "timer.h"

int ticks = 0;
int seconds = 0; 
int minutes = 0;
int drawbar = false;

void PITIRQ0(struct InterruptRegisters *r)
{
    ticks += 1;

    if (ticks % 20 == 0)
    {
        seconds++;
    }
}

void sleep(int seconds)
{
    int startticks = ticks;
    
    while (ticks < startticks + (startticks * (seconds * 10)))
    {
        // NOTHING HERE
    }
}

void loadbar()
{
    while (seconds % 10 != 0)
    {
        int totaltime = 10;
        int timeleft = totaltime - seconds;

        int wbar = (seconds * 100) / totaltime;

        for (int y = 0; y < 16; y++)
        {
            for (int x = 0; x < wbar; x++)
            {
                setpixel(x + (WSCREEN / 2 - 38), y + (HSCREEN / 2 + 70), 0xFF00FF00);
            }
        }
    }
}

void inittimer()
{
    irqinstallhan(0x00, &PITIRQ0);

    DWORD divisor = 1193180 / 100;

    outb(0x43, 0x36);
    outb(0x40, (BYTE)(divisor & 0xFFFF));
    outb(0x40, (BYTE)((divisor & 0xFFFF) & 0xFF));
}