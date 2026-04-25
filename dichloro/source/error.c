/*
RED SCREEN OF DEATH LIKE NOVA OS
*/

#include "variables.h"
#include "video.h"
#include "graphics.h"
#include "text.h"
#include "idt.h"

#include "error.h"

void errscreen(const char* error)
{
    drawpixel(0, 0, WSCREEN, HSCREEN, 0xFFFF0000);

    SetCursorX(0x00);
    SetCursorY(0x00);

    Print("DichloroOS ERROR, SHUTDOWN YOUR SYSTEM\n\n", 0xFFFFFFFF);

    Print("Problem: ", 0xFFFFFFFF);
    Print(error, 0xFFFFFFFF);

    Print("\n\nIf the problem persist, contact OsJanelas (SuperNaray or DaniloSensei), the creators of this OS via discord. \n\nRegisters: ", 0xFFFFFFFF);
}