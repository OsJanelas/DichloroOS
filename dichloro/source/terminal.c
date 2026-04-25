// VARIABLES
#include "variables.h"
#include "math.h"
#include "memfilesys.h"
#include "ports.h"

// ELF
#include "elf.h"

// GRAPHICS MODE
#include "video.h"
#include "graphics.h"
#include "text.h"

// FILE SYSTEM
#include "timer.h"
#include "filesystem.h"

// HARDWARE
#include "processor.h"
#include "memory.h"
#include "virtmem.h"
#include "date.h"
#include "allocate.h"
#include "memory.h"
#include "board.h"

// DRIVERS
#include "mouse.h"
#include "kb.h"
#include "power.h"

// INTERFACE
#include "winsys.h"
#include "gui.h"
#include "userspace.h"

// TINY GL
#include "tinygl/include/GL/tg_gl.h"
#include "tinygl/include/zbuffer.h"
#include "box.h"

// SHELL MAIN FILES
#include "notepad.h"
#include "shell.h"

char* currentdir = "Disk/root>";

// PROGRAM
extern char scroll[];
extern char cube[];

void welcomemsg()
{
    SetCursorX(0x00);
    SetCursorY(0x00);

    Print("Welcome to DichloroOS!\n\n", 0xFF00FF00)
    Print("Type 'help' to use shell\n\n", 0xFFFFFFFF)
    Print("Type 'gui' to use UI\n\n", 0xFFFFFFFF)
}

void processcomand(char* command)
{
        char cmd[16] = {0};
    char args[2][16] = {{0}};
    int cmdIndex = 0;
    int argIndex = 0;
    int argCount = 0;
    int i = 0;

    while (command[i] != ' ' && command[i] != '\0')
    {
        cmd[cmdIndex++] = command[i++];
    }
    cmd[cmdIndex] = '\0';

    if (command[i] == ' ') i++;

    while (command[i] != '\0' && argCount < 2)
    {
        if (command[i] == ' ')
        {
            args[argCount][argIndex] = '\0';
            argCount++;
            argIndex = 0;
        }
        else
        {
            args[argCount][argIndex++] = command[i];
        }
        i++;
    }

    if (argIndex > 0 && argCount < 2)
    {
        args[argCount][argIndex] = '\0';
        argCount++;
    }

    if (GetCursorY() >= 448)
    {
        SetCursorX(0x00);
        SetCursorY(0x00);
        clearscreen();
    }

    if (strcmp(cmd, "help") == 0x00)
    {
        Print("\n\nclear - ", 0xFF00FF00);         Print("Clear the screen.", 0xFFFFFFFF);
        Print("\nabout - ", 0xFF00FF00);           Print("More about DichloroOS.", 0xFFFFFFFF);
        Print("\necho - ", 0xFF00FF00);            Print("Show a message on terminal.", 0xFFFFFFFF);
        Print("\nshutdown - ", 0xFF00FF00);        Print("Shutdown the system.", 0xFFFFFFFF);
        Print("\nrestart - ", 0xFF00FF00);         Print("Reboot the system.", 0xFFFFFFFF);
        Print("\nspecif - ", 0xFF00FF00);          Print("Show system specs.", 0xFFFFFFFF);
        Print("\nboot - ", 0xFF00FF00);            Print("Starts GUI.", 0xFFFFFFFF);
        Print("\nmkfile - ", 0xFF00FF00);          Print("Create a file.", 0xFFFFFFFF);
        Print("\nread - ", 0xFF00FF00);            Print("Read file content.", 0xFFFFFFFF);
        Print("\ninfo - ", 0xFF00FF00);            Print("Show a file informations.", 0xFFFFFFFF);
        Print("\nrename - ", 0xFF00FF00);          Print("Rename a file.", 0xFFFFFFFF);
        Print("\ndeletefile - ", 0xFF00FF00);      Print("Delete a file", 0xFFFFFFFF);
        Print("\ndisk - ", 0xFF00FF00);            Print("List all files and folders in current local.", 0xFFFFFFFF);
        Print("\nmakedir - ", 0xFF00FF00);         Print("Create a dir.", 0xFFFFFFFF);
        Print("\ndeletefold - ", 0xFF00FF00);      Print("Delete a existing directory.", 0xFFFFFFFF);
        Print("\nchangesys - ", 0xFF00FF00);       Print("Change the current directory.", 0xFFFFFFFF);
        Print("\nnote - ", 0xFF00FF00);            Print("Notepad.", 0xFFFFFFFF);
        Print("\nbox - ", 0xFF00FF00);             Print("TinyGL program", 0xFFFFFFFF);
    }
    else if (strcmp(cmd, "about") == 0x00)
    {
        Print("\n\nDevelopers: ", 0xFF00FF00);    Print("OsJanelas\n", 0xFFFFFFFF);
        Print("First Build Date: ", 0xFF00FF00); Print("03/28/2026\n\n", 0xFFFFFFFF);

        Print("This OS is being made for education purposes\n", 0xFFFFFFFF);
        Print("Also, DichloroOS is the most hardest and challenging project I've coded before.\n", 0xFFFFFFFF);
        Print("Fun fact: This OS is our first with ELF32 and files suport (we named the GUI 'Chemical UI')\n\n", 0xFFFFFFFF);

        Print("Special Credits: ", 0xFFFFFF00); Print("ArTicZera/JhoPro", 0xFFFFFFFF);
    }

    else if (strcmp(cmd, "clear") == 0x00)
    {
        SetCursorX(0x00);
        SetCursorY(0x00);
        ClearScreen();
    }
    else if (strcmp(cmd, "echo") == 0x00)
    {
        Print("\n\n", 0x00);
        Print(args[0], 0x0F);
    }
    else if (strcmp(cmd, "shutdown") == 0x00)
    {
        shutdown();
    }
    else if (strcmp(cmd, "restart") == 0x00)
    {
        restart();
    }
    else if (strcmp(cmd, "specif") == 0x00)
    {
        Print("\n\n", 0x0F);
        Print(" #################            \n", 0xFF00FFFF);
        Print(" ###################          \n", 0xFF00FFFF);
        Print(" #######     #######          \n", 0xFF00FFFF);
        Print(" #######     #######          System dir:", 0xFF00FFFF); Print("localdiks/dichloro\n", 0xFFFFFFFF);
        Print(" #######     #######          \n", 0xFF00FFFF);
        Print(" #######     #######          System: ", 0xFF00FFFF); Print("DichloroOS x86\n", 0xFFFFFFFF);
        Print(" #######     #######          Kernel version: ", 0xFF00FFFF); Print("0.1\n", 0xFFFFFFFF);
        Print(" ###################          Resolution: ", 0xFF00FFFF); Print("800x600\n", 0xFFFFFFFF);
        Print(" #################            Video Mode: ", 0xFF00FFFF); Print("VESA BIOS Extensions\n", 0xFFFFFFFF);
        Print("       /\                     CPU: ", 0xFF00FFFF); showcpuname();
        Print("      |\/|                    Date: ", 0xFF00FFFF); getcmosdate(); Print("\n", 0x00);
        Print("      | ||                    \n", 0xFF00FFFF);
        Print("      | ||                    ", 0xFF00FFFF);
        Print("\f\f", 0xFF0E1A14);
        Print("\f\f", 0xFF1B2A21);
        Print("\f\f", 0xFF23382C);
        Print("\f\f", 0xFF2F4A38);
        Print("\f\f", 0xFF3F5F49);
        Print("\f\f", 0xFF5F7566);
        Print("\f\f", 0xFF8A9A90);
        Print("\f\f\n", 0xFFB7C0B9);
        Print("       \/       ", 0xFF00FFFF);
    }
    else if (strcmp(cmd, "boot") == 0x00)
    {
        userspacestate(0x01);
    }
    else if (strcmp(cmd, "makefile") == 0x00)
    {
        createfile(args[0], args[1], sizeof(args[1]), PERM_R | PERM_W);
    }
    else if (strcmp(cmd, "makedir") == 0x00)
    {
        createdir(args[0]);
    }
    else if (strcmp(cmd, "read") == 0x00)
    {
        BYTE buffer[128];
        DWORD size;

        int result = readfile(args[0], buffer, &size);
        
        if (result != -1)
        {
            Print("\n\n", 0x00);
            Print(buffer, 0xFFFFFFFF);
        }
    }
    else if (strcmp(cmd, "info") == 0x00)
    {
        fileinfo(args[0]);
    }
    else if (strcmp(cmd, "rename") == 0x00)
    {
        renamefile(args[0], args[1]);
    }
    else if (strcmp(cmd, "deletefile") == 0x00)
    {
        deletefile(args[0]);
    }
    else if (strcmp(cmd, "deletefold") == 0x00)
    {
        deletedir(args[0]);
    }
    else if (strcmp(cmd, "disk") == 0x00)
    {
        listfiles();
        show();
    }
    else if (strcmp(cmd, "changesys") == 0x00)
    {
        changedir(args[0]);
    }
    else if (strcmp(cmd, "run") == 0x00)
    {
        processrun(args[0]);
    }
    else if (strcmp(cmd, "ping") == 0x00)
    {
        sendping(args[0]);
    }
    else if (strcmp(cmd, "note") == 0x00)
    {
        startnotepad(args[0]);
    }
    else if (strcmp(cmd, "box") == 0x00)
    {
        ZBuffer *zb = ZB_open(WSCREEN, HSCREEN, ZB_MODE_RGBA, (void*)GetFramebuffer());

        glInit(zb);

        init();
        reshape(WSCREEN, HSCREEN);

        INT i = 0;
        
        while (i < 100)
        {
            idle();

            i++;
        }

        ZB_close(zb);
        glEnd();
    }
    else
    {
        Print("\n\nERROR: Invalid command. Try again or type 'help'", 0xFFFF0000);
    }

    Print("\n\n", 0x00);
    showcurrentfolder();
}

void processrun(char* process)
{
    if (strcmp(process, "scroll.exe") == 0x00)
    {
        exeelf(scroll);
    }
    else if (strcmp(process, "cube.exe") == 0x00)
    {
        exeelf(cube);
    }
    else
    {
        Print("\n\nERROR: This program don't exist", 0xFFFF0000);
    }
}

void startshell()
{
    welcomemsg();

    showactualdir();

    getkbstate(0x02);
}