#include "variables.h"
#include "memory.h"
#include "text.h"
#include "graphics.h"

#include "gui.h"
#include "winsys.h"
#include "guishell.h"

void shellwindow(char* command)
{
    char cmd[16] = {0};
    int cmdIndex = 0;
    int i = 0;

    while (command[i] != ' ' && command[i] != '\0')
    {
        cmd[cmdIndex++] = command[i++];
    }

    cmd[cmdIndex] = '\0';

    if (strcmp(cmd, "NULL") == 0x00)
    {
    }
    else
    {
        int msgW = 400;
        int msgH = 110;

        int x = 320 - 200;
        int y = 240 - 55;

        WINDOW msgbox = { "Run - ERROR", 0x13, x, y, msgW, msgH  };

        messagebox(msgbox, ICON_ERROR, "RUNNING ERROR:\n\nThis program not exist", 0x00);
    }
}