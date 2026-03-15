#include "include/variables.h"
#include "include/ports.h"
#include "interrupts/idt.h"
#include "text.h"
#include "shell.h"
#include "graphics.h"

#include "gui.h"
#include "userspace.h"
#include "login.h"

#include "kb.h"

int allowinput = 1;
int enabletext = 0;

int shift = 0;
int caps = 0;

char commandbuff[50];
int commandlenght = 0;

// ALL KEYS
const char* keymap[][2] = 
{
    { " ", " " }, { " ", " " },
    { "1", "!" }, { "2", "@" }, { "3", "#" }, { "4", "$" }, { "5", "%" },
    { "6", "^" }, { "7", "&" }, { "8", "*" }, { "9", "(" }, { "0", ")" },
    { "-", "_" }, { "=", "+" }, { "\b", "\b" }, { " ", " " },
    { "q", "Q" }, { "w", "W" }, { "e", "E" }, { "r", "R" }, { "t", "T" },
    { "y", "Y" }, { "u", "U" }, { "i", "I" }, { "o", "O" }, { "p", "P" },
    { "[", "{" }, { "]", "}" }, { "\n", "\n" }, { " ", " " },
    { "a", "A" }, { "s", "S" }, { "d", "D" }, { "f", "F" }, { "g", "G" },
    { "h", "H" }, { "j", "J" }, { "k", "K" }, { "l", "L" }, { ";", ":" },
    { "'", "\"" }, { "`", "~" }, { " ", " " }, { "\\", "|" },
    { "z", "Z" }, { "x", "X" }, { "c", "C" }, { "v", "V" }, { "b", "B" },
    { "n", "N" }, { "m", "M" }, { ",", "<" }, { ".", ">" }, { "/", "?" },
    { " ", " " }, { " ", " " }, { "*", " " }, { " ", " " }, { " ", " " }
};

void keyboardstate(int state)
{
    enabletext = state;
}

static BYTE keyDown[128] = {0};

void handlechar(int scan) 
{
    if (commandLength >= 49) return;

    char c = (shift || caps) ? keymap[scan][1][0] : keymap[scan][0][0];

    commandbuff[commandlength] = c;

    if (enableText == 3) // PASSWORD
    {
        PrintOut('*', 0xFFFFFFFF);
        commandlength++;
    }
    else if (enabletext == 2) // SHELL
    {
        PrintOut(c, 0xFFFFFFFF);
        commandlength++;
    }
}

void keyboardhandler()
{
    //Read scan code
    uint8_t scan = inb(0x60);

    //Check if its pressed
    uint8_t isPress = !(scan & 0x80);
    scan &= 0x7F;

    if (isPress)
    {
        if (keydown[scan])
            return;
        keydown[scan] = 1;
    }
    else
    {
        keydown[scan] = 0;
        return;
    }

    if (allowinput)
    {
        switch (scan)
        {   
            case 0x0E:
                if (isPress)
                {
                    setcursorX(getcursorX() - 8);

                    Print("\f", 0x00000000);

                    setcursorX(getcursorX() - 8);

                    commandlength--;
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
                    caps = !caps;
                }
                break;

            //Enter
            case 0x1C:
                if (isPress) 
                {
                    commandbuff[commandlength] = '\0';

                    if (enableText == 2) 
                    {
                        ProcessShellCMD(commandbuff);
                    } 
                    else if (enableText == 3) 
                    {
                        SetPassword(commandbuff);
                    }
                    else if (enableText == 4)
                    {
                        Print("\n", 0x00);
                    }

                    commandLength = 0;
                }

                break;

            case 0x5B:
            case 0x5C:
                break;

            default:
                if (isPress) 
                {
                    HandleCharacter(scan);
                    break;
                }
                
                break;
        }
    }
}

void initkb()
{
    irqinstallhandler(0x01, &keyboardhandler)
}