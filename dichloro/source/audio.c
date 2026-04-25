/*
    THIS CODE IS FOR A AUDIO BOARD DRIVER
    
    AUDIO BOARD: SOUND BLASTER 16
*/

#include "variables.h"
#include "ports.h"
#include "text.h"

#include "audio.h"

int playing;

void resetaudio()
{
    outb(0x226, 0x01);
    outb(0x226, 0x00);
}

void writedsp(BYTE cmd)
{
    while ((inb(0x22A) & 0x80) != 0);
    outb(0x22C, cmd);
}

void audiohandler()
{
    inb(0x22E);
    outb(0x20, 0x20);
    playing = 0x00;
}

void initaudioboard()
{
    writedsp(0xD1);
}

void disableaudio()
{
    writedsp(0xD3);
}

void setupdma(LPBYTE buffer, WORD size)
{
    uint64_t addr = (ULONG)buffer;

    outb(0x0A, 0x05);
    outb(0x0C, 0x00);
    outb(0x0B, 0x49);
    outb(0x02, addr & 0xFF);
    outb(0x02, (addr >> 8) & 0xFF);
    outb(0x83, (addr >> 16) & 0xFF);
    outb(0x03, (size - 1) & 0xFF);
    outb(0x03, (size - 1) >> 8);
    outb(0x0A, 0x01);
}

void audioplay(LPBYTE buffer, WORD size)
{
    playing = 1;

    setupdma(buffer, size);

    writedsp(0x40);
    writedsp(256 - (1000000 / 8000));

    writedsp(0x14);
    writedsp((size - 1) & 0xFF);
    writedsp((size - 1) >> 8);

    for (int i = 0; i < 10000000; i++);

    playing = 0x00;
}

void setupaudioboard()
{
    resetaudio();
    initaudioboard();
}

void playbytebeat(LPBYTE buffer, WORD size)
{
    audioplay(buffer, size);

    while (playing)
    disableaudio();
}