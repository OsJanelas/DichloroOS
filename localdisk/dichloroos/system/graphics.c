#include "include/variables.h"
#include "timer.h"

#include "graphics.h"

DWORD vidmem;
DWORD pitch;

void initgraphics(LPDWORD mbinfo)
{
    vidmem = ((QWORD)mbinfo[23] << 32) | mbinfo[22];
    pitch = mbinfo[24];
}

DWORD getframebuffer()
{
    return vidmem;
}

DWORD getpitch()
{
    return pitch;
}

void setpixel(int x, int y, DWORD color)
{
    LPDWORD framebuffer = (LPDWORD) vidmem;

    framebuffer[x + (y * pitch / 4)] = color;
}

DWORD getpixel(int x, int y)
{
    LPDWORD framebuffer = (LPDWORD) vidmem;

    return framebuffer[x + (y * pitch / 4)];
}

void clearscreen(void)
{
    for (int y = 0; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            setpixel(x, y, 0x00);
        }
    }
}

void drawpixel(int x, int y, int w, int h, DWORD color)
{
    for (int i = y; i < y + h; i++)
    {
        for (int j = x; j < x + w; j++)
        {
            setpixel(j, i, color);
        }
    }
}