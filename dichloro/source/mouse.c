#include "variables.h"
#include "ports.h"
#include "graphics.h"
#include "video.h"
#include "idt.h"
#include "text.h"
#include "winsys.h"
#include "timer.h"
#include "gui.h"

#include "cursor.h"
#include "mouse.h"

DWORD mousearea[WCURSOR * HCURSOR];
DWORD mousecurafter[WCURSOR * HCURSOR];
DWORD mouseprevious[WCURSOR * HCURSOR];

BYTE mousedata;
BYTE mousecycle;
BYTE mousepacket[3];

int mousepressed = 0;

int prevmousex = 0;
int prevmousey = 0;

int mouseX = 0;
int mouseY = 0;

void getstate(int* x, int* y, int* pressed)
{
    *x = mouseX;
    *y = mouseY;
    *pressed = mousepressed;
}

void drawcursor(DWORD color)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        int x = 0;

        for (int i = 0; i < 2; i++)
        {
            BYTE byte = cursor[y * 2 + i];

            for (int j = 7; j >= 0; j--)
            {
                if (byte & (1 << j))
                {
                    setpixel(mouseX + x, mouseY + y, color);
                }
                x++;
            }
        }
    }
}

void savearea()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            DWORD color = getpixel(mouseX + x, mouseY + y);

            mousearea[y * WCURSOR + x] = color;
        }
    }
}

void resetpreviouscursor(int x, int y)
{
    for (int yy = 0; yy < HCURSOR; yy++)
    {
        for (int xx = 0; xx < WCURSOR; xx++)
        {
            setpixel(x + xx, y + yy, mousearea[yy * WCURSOR + xx]);
        }
    }
}

void mousewait()
{
    while (inb(0x64) & 0x02);
}

void cleartrails(int prevmousex, int prevmousey)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            setpixel(prevmousex + x, prevmousey + y, mousearea[y * WCURSOR + x]);
        }
    }
}

void mousehandler(struct InterruptRegisters *r)
{
    mousedata = inb(0x60);

    if (mousecycle == 0 && !(mousedata & 0x08))
    {
        return;
    }

    mousepacket[mousecycle++] = mousedata;

    if (mousecycle == 3)
    {
        mousecycle = 0;

        mousepressed = (mousepacket[0] & 0x1);

        int deltaX = (int)((char)mousepacket[1]);
        int deltaY = (int)((char)mousepacket[2]);
        int prevmousex = mouseX;
        int prevmousey = mouseY;

        mouseX += deltaX;
        mouseY -= deltaY;

        mouseX = (mouseX < 0) ? 0 : ((mouseX > WSCREEN - 16) ? WSCREEN - 16 : mouseX);
        mouseY = (mouseY < 0) ? 0 : ((mouseY > HSCREEN - 16) ? HSCREEN - 16 : mouseY);

        cleartrails(prevmousex, prevmousey);
        savearea();
        resetpreviouscursor(prevmousex, prevmousey);
        drawcursor(0xFFFFFFFF);
    }
}

void initmouse()
{
    outb(0x64, 0xA7);
    outb(0x64, 0xA8);

    while (inb(0x64) & 1)
    {
        inb(0x60);
    }

    irqinstallhan(12, &mousehandler);

    outb(0x64, 0x20);
    BYTE status = (inb(0x60) | 2);
    outb(0x64, 0x60);
    outb(0x60, status);

    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
}
