#include "variables.h"
#include "winsys.h"
#include "video.h"
#include "memfilesys.h"
#include "ports.h"
#include "idt.h"
#include "text.h"
#include "shell.h"
#include "notepad.h"
#include "graphics.h"
#include "filesystem.h"
#include "userspace.h"
#include "gui.h"
#include "login.h"

#include "map.h"
#include "kb.h"

extern char* file;

int allowinput = 1;
int enabletext = 0;

int shift = 0;
int ctrl = 0;
int alt = 0;
int capslock = 0;

char commandbuff[50];
int commandlenght = 0;

char notepadbuff[1024];
int notepadlenght = 0;

void getkbstate(int state)
{
    enabletext = state;
}

void handlechar(int scan)
{
    if (commandlenght >= 49 || notepadlenght >= 1023) return;

    char c = (shift || capslock) ? keymap[scan][1][0] : keymap[scan][0][0];

    commandbuff[commandlenght] = c;
    notepadbuff[notepadlenght] = c;

    if (enabletext == 3)
    {
        printout('$', 0xFFFFFFFF);

        commandlenght++;
    }

    else if (enabletext == 2)
    {
        printout(c, 0xFFFFFFFF);

        commandlenght++;
        notepadlenght++;
    }

    else if(enabletext == 1)
    {
        printout(c, 0xFFFFFFFF);

        commandlenght++;
    }
}

void keyboardhandler(struct InterruptRegisters *r)
{
        uint8_t scan = inb(0x60);

    //Check if its pressed
    uint8_t ispress = !(scan & 0x80);
    scan &= 0x7F;

    if (ispress)
    {
        if (keydown[scan])
            return;
        keydown[scan] = 1;
    }
    else
    {
        keydown[scan] = 0;
    }

    if (allowinput)
    {
        switch (scan)
        {
            case 0x01:
                if (ispress)
                {
                    closenotepad();
                }
                break;

            case 0x0E:
                if (ispress)
                {
                    SetCursorX(GetCursorX() - 8);

                    Print("\f", 0x00000000);

                    SetCursorX(GetCursorX() - 8);

                    commandlenght--;
                }
                break;

            case 0x1D:
                ctrl = ispress;
                break;
            case 0x1F:
                if (ispress & ctrl)
                {
                    createtextfile(file, (LPBYTE)notepadbuff, notepadlenght);
                    closenotepad();
                    break;
                }
                if (ispress) 
                {
                    handlechar(scan);
                    break;
                }
                break;

            //Shift
            case 42:
            case 54:
                shift = ispress;
                break;

            //Caps lock
            case 58:
                if (ispress)
                {
                    capslock = !capslock;
                }
                break;

            //Enter
            case 0x1C:
                if (ispress) 
                {
                    commandbuff[commandlenght] = '\0';

                    if (enabletext == 2) 
                    {
                        processcomand(commandbuff);
                    } 
                    else if (enabletext == 3) 
                    {
                        password(commandbuff);
                    }
                    else if (enabletext == 4)
                    {
                        Print("\n", 0x00);
                    }

                    commandlenght = 0;
                }

                break;

            case 0x5B:
            case 0x5C:
                break;

            default:
                if (ispress) 
                {
                    handlechar(scan);
                    break;
                }
                
                break;
        }
    }
}

void initkeyboard()
{
    irqinstallhan(0x01, &keyboardhandler);
}