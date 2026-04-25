#include "variables.h"
#include "timer.h"

#include "graphics.h"
#include "video.h"

DWORD vidmemory;
DWORD pitch;

void initgraphics(LPDWORD mbinfo)
{
    vidmemory = ((QWORD)mbinfo[23] << 32) | mbinfo[22];
    pitch = mbinfo[24];
}

DWORD getframebuff()
{
    return vidmemory;
}

DWORD getpitch()
{
    return pitch;
}

void setpixel(int x, int y, DWORD color)
{
    LPDWORD framebuffer = (LPDWORD) vidmemory;

    framebuffer[x + (y * pitch / 4)] = color;
}

DWORD getpixel(int x, int y)
{
    LPDWORD framebuffer = (LPDWORD) vidmemory;

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