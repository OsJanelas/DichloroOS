#include "include/variables.h"
#include "include/ports.h"
#include "graphics.h"
#include "interrupts/idt.h"
#include "text.h"
#include "timer.h"
#include "gui.h"

#include "cursor.h"
#include "mouse.h"

DWORD mousearea[WCURSOR * HCURSOR];
DWORD mousecursorafter[WCURSOR * HCURSOR];
DWORD mouseprevious[WCURSOR * HCURSOR];

BYTE mousedata;
BYTE mousecycle;
BYTE mousepacket[4];

int mousepressed = 0;

int prevmouseX = 0;
int prevmouseY = 0;

int mouseX = 0;
int mouseY = 0;

void getmousestate(int* x, int*, y, int*, pressed);
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

void savemousearea()
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

void restorepreviewcursor()
{
    for (int y = 0; y < HCURSOR; y++)
    {
        
        for (int x = 0; x < WCURSOR; x++)
        {
            setpixel(mouseX + x, mouseY + y, mousearea[y * WCURSOR + x]);
        }        
    }
}

void mousewait()
{
    while (inb(0x64) & 0x02);
}

void cleartrails(int prevmouseX, int prevmouseY)
{
    for (int y = 0; y < HCURSOR; y++)
    {
        for (int x = 0; x < WCURSOR; x++)
        {
            setpixel(prevmouseX + x, prevmouseY + y, mousearea[y * WCURSOR + x]);
        }
    }
}

void mousehandler()
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

        int prevmouseX = mouseX;
        int prevmouseY = mouseY;

        mouseX += deltaX;
        mouseY -= deltaY;

        mouseX = (mouseX < 0) ? 0 : ((mouseX > WSCREEN - 16) ? WSCREEN - 16 : mouseX);
        mouseY = (mouseY < 0) ? 0 : ((mouseY > HSCREEN - 16) ? HSCREEN - 16 : mouseY);

        cleartrails(prevmouseX, prevmouseY);
        savemousearea();
        restorepreviewcursor(prevmouseX, prevmouseY);
        drawmouse(0xFFFFFFFF);
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

    irqinstallhandler(12, &mousehandler);

    outb(0x64, 0x20);
    BYTE status = (inb(0x60) | 2);
    outb(0x64, 0x60);
    outb(0x60, status);

    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
}