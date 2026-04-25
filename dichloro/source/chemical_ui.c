#include "variables.h"
#include "idt.h"
#include "gl.h"
#include "date.h"
#include "mouse.h"
#include "allocate.h"
#include "timer.h"
#include "audio.h"
#include "video.h"
#include "graphics.h"
#include "text.h"
#include "memory.h"
#include "memfilesys.h"
#include "filesystem.h"
#include "kb.h"

#include "guishell.h"
#include "desktop.h"
#include "winsys.h"
#include "gui.h"
#include "userspace.h"
#include "login.h"

WINDOW start = { "", 0xFF1A1A1A, 0, HSCREEN - 285, 150, 250, 0 };
WINDOW* winmanager;

extern DWORD error[];
extern DWORD warning[];
extern DWORD info[];
extern DWORD question[];
extern DWORD user[];
extern DWORD user2[];
extern DWORD terminal[];
extern DWORD non[];
extern DWORD txt[32 * 32];
extern DWORD dir[32 * 32];
extern DWORD exe[32 * 32];

int inuserspace = 0x00;

int startmenuopen = 0;
int ismaximizedopen = 0;   
int wasmousepressed = 0;

int currentpid = 0;
int totalwindows = 0;

DWORD startbuffer[150 * 200];

DWORD* backbuffer;

extern char usericon[];

char passkey[16];
int loginloop = 1;

extern BYTE bootscr[];
extern BYTE wallpp[];

extern BYTE bootsound[];

item items[MAXITEMS];
int totalitems = 0;

// DESKTOP SETTINGS
item* getclickeditem(int x, int y)
{
    for (int i = 0; i < totalitems; i++)
    {
        if (x >= items[i].x && x < items[i].x + 100 && y >= items[i].y && y < items[i].y + 85)
        {
            return &items[i];
        }
    }

    return 0x00;
}

void setdektopindex(char* name,  int type, int x, int y)
{
    if (totalitems < MAXITEMS)
    {
        strncpy(items[totalitems].name, name, 15);
        items[totalitems].name[15] = '\0';
        items[totalitems].type = type;
        items[totalitems].x = x;
        items[totalitems].y = y;
    }
}

// LOGIN SCREEN SETTINGS
void loginscreen()
{
    drawpixel(0, 0, WSCREEN, HSCREEN, 0xFF2F2F2F);

    int w = 300;
    int h = 150;

    int x = WSCREEN / 2 - w / 2;
    int y = HSCREEN / 2 - h / 2 - 50;

    y += h / 4;
    
    for (int ys = 0; ys < 32; ys++)
    {
        for (int xs = 0; xs < 32; xs++)
        {
            BYTE color = usericon[ys * 32 + (xs + 8)];

            if (color != 0x00 && color != 28)
            {
                setpixel(x + xs * 2, y + ys * 2, color ? 0xFFFFFFFF : 0x00);
                setpixel(x + xs * 2 + 1, y + ys * 2, color ? 0xFFFFFFFF : 0x00);
                setpixel(x + xs * 2, y + ys * 2 + 1, color ? 0xFFFFFFFF : 0x00);
                setpixel(x + xs * 2 + 1, y + ys * 2 + 1, color ? 0xFFFFFFFF : 0x00);
            }
        }
    }

    SetCursorX(x + 70);
    SetCursorY(y + 20);

    Print("Welcome, user", 0xFFFFFFFF);

    SetCursorX(x);
    SetCursorY(y + 80);

    Print("Create your password", 0xFFFFFFFF);
    drawrect(x, y + 100, 300, 20, 5, 0xFF1A1A1A);

    SetCursorX(x + 5);
    SetCursorY(y + 100);

    while (loginloop)
    {

    }
}

void password(char* pass)
{
    memorycopy(passkey, pass, sizeof(passkey));

    loginloop = 0x00;
}

// USERSPACE SETTINGS
void wallpaper()
{
    int bytes_per_pixel = 3;
    int bytes_per_row = WSCREEN * bytes_per_pixel;
    int padding = (4 - (bytes_per_row % 4)) % 4;

    for (int y = 0; y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            int img_y = (HSCREEN - 1 - y);
            int index = (img_y * bytes_per_row) + (x * bytes_per_pixel) + (img_y * padding);

            BYTE b = wallpp[index];
            BYTE g = wallpp[index + 1];
            BYTE r = wallpp[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            setpixel(x - 18, y, color);
        }
    }
}

void taskbar()
{
    DWORD* blurbuff = allocatememory(WSCREEN * HSCREEN * 4);

    blurregion(backbuffer, blurbuff, 0 , HSCREEN - 35, WSCREEN, 35, 1);

    for(int y = HSCREEN - 35; y < HSCREEN; y++)
    {
        memorycopy(&backbuffer[y * WSCREEN], &blurbuff[y * WSCREEN], WSCREEN * 4);
    }
}

void savestartarea()
{
    for (int y = HSCREEN - (start.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < start.w; x++)
        {
            startbuffer[y * getpitch() / 4 + x] = getpixel(x, y);
        }
    }
}

void resetstartarea()
{
    for (int y = HSCREEN - (start.h + 35); y < HSCREEN - 35; y++)
    {
        for (int x = 0; x < start.w; x++)
        {
            setpixel(x, y, startbuffer[y * getpitch() / 4 + x]);
        }
    }
}

void startbutton(int x, int y, int pressed)
{
    int startX = 0;
    int endX = 100;

    int startY = HSCREEN - 35;
    int endY = HSCREEN;

    if (pressed && !wasmousepressed && x >= startX && x < endX && y >= startY && y < endY)
    {
        if (startmenuopen)
        {
            resetstartarea();
            startmenuopen = 0;
        }
        else
        {
            savestartarea();
            startmenu(start);
            startmenuopen = 1;
        }
    }
    
    wasmousepressed = pressed;
}

void updateexplorer()
{
    wallpaper();
    icons();
    taskbar();

    for (int y = HSCREEN - 35; y < HSCREEN; y++)
    {
        for (int x = 0; x < WSCREEN; x++)
        {
            setpixel(x, y, backbuffer[y * WSCREEN + x]);
        }
    }

    SetCursorX(30);
    SetCursorY(HSCREEN - 25);
    Print("start", 0xFFFFFFFF);

    SetCursorX(WSCREEN - 65);
    SetCursorY(HSCREEN - 25);
    getcmosdate();
}

void userspacestate(int state)
{
    if (state == FALSE)
    {
        inuserspace = 0x00;
    }
    else
    {
        inuserspace = 0x01;
    }
}

void icons()
{
    filesystem* fs = (filesystem*) FILESYSTEMADREES;
    
    int index = 0;

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (fs->root.subdirs[i] != NULL && fs->root.subdirs[i]->name[0] != '\0')
        {
            int x = 10;
            int y = 10 + (index * 76);
            drawicon(ICON_DIR, fs->root.subdirs[i]->name, x, y);
            setdektopindex(fs->root.subdirs[i]->name, 0x00, x, y);

            index++;
        }
    }

    for (int i = 0; i < MAXFILES; i++)
    {
        if (fs->root.files[i].filename[0] != '\0')
        {
            char* filename = fs->root.files[i].filename;
            const char* extension = NULL;

            for (const char* p = filename; *p != '\0'; p++) 
            {
                if (*p == '.') 
                {
                    extension = p + 1;
                }
            }

            EXTICON icon;

            if (extension != NULL) 
            {
                if (strcmp(extension, "txt") == 0) 
                {
                    icon = ICON_TEXT;
                } 
                else if (strcmp(extension, "exe") == 0) 
                {
                    icon = ICON_EXE;
                } 
                else 
                {
                    icon = ICON_DEFAULT;
                }
            } 
            else 
            {
                icon = ICON_DEFAULT;
            }

            int x = 64;
            int y = 20 + (index * 76);
            drawicon(icon, filename, x, y);
            setdektopindex(filename, 0x01, x, y);

            index++;
        }
    }
}

void bootsector()
{
    int bytes_per_pixel = 3;
    int bytes_per_row = 128 * bytes_per_pixel;
    int padding = (4 - (bytes_per_row % 4)) % 4;

    for (int y = 0; y < 128; y++)
    {
        for (int x = 0; x < 128; x++)
        {
            int img_y = (129 - 1 - y);
            int index = (img_y * bytes_per_row) + ((x - 110) * bytes_per_pixel) + (img_y * padding);
            
            int dx = WSCREEN / 2 - 64;
            int dy = HSCREEN / 2 - 128;

            BYTE b = bootscr[index];
            BYTE g = bootscr[index + 1];
            BYTE r = bootscr[index + 2];

            DWORD color = (0xFF << 24) | (r << 16) | (g << 8) | b;

            setpixel(x + dx, y + dy, color);
        }
    }

    loadbar();
    clearscren();
}

void userspace()
{
    while (!inuserspace)
    {

    }

    getkbstate(3);

    loginscreen();

    getkbstate(1);

    updateexplorer();

    int lastpressed = 0;

    while (TRUE)
    {
        int x, y, pressed;
        getstate(&x, &y, &pressed);

        startbutton(x, y, pressed);

        handleclick(x, y, pressed);

        dragging(x, y, pressed);
    }
}

// GUI CONFIG
void startmenu(WINDOW window)
{
    drawpixel(window.x, window.y, window.w, window.h, window.color);

    SetCursorX(window.x / 2 + 50);
    SetCursorY(window.y + 22);

    Print("User", 0xFFFFFFFF);
}

void drawicon(EXTICON exiticon, char* name, int posX, int posY)
{
        const DWORD* iconbitmap = NULL;

    switch (exiticon)
    {
        case ICON_TEXT:    iconbitmap = txt;  break;
        case ICON_DIR:     iconbitmap = dir;  break;
        case ICON_EXE:     iconbitmap = exe;  break;
        case ICON_DEFAULT: iconbitmap = non;  break;
        case ICON_USER2:   iconbitmap = user2; break;
        default: return;
    }

    for (int y = 0; y < 32; y++)
    {
        int srcY = 31 - y;

        for (int x = 0; x < 32; x++)
        {
            if (iconbitmap[srcY * 32 + x] == 0x00)
            {
                continue;
            }

            DWORD color = iconbitmap[srcY * 32 + x];
            setpixel(posX + x + 16, posY + y, color);
        }
    }

    SetCursorX(posX + 20);
    SetCursorY(posY + 36);
    Print(name, 0xFFFFFFFF);
}

void drawmessageicon(ICON icon, int posX, int posY)
{
    const DWORD* iconbitmap = NULL;

    switch (icon)
    {
        case ICON_ERROR:     iconbitmap = error;    break;
        case ICON_WARNING:   iconbitmap = warning;  break;
        case ICON_INFO:      iconbitmap = info;     break;
        case ICON_QUESTION:  iconbitmap = question; break;
        case ICON_USER1:     iconbitmap = user;     break;
        case ICON_TERMINAL:  iconbitmap = terminal; break;
        case ICON_NONE:
        default: return;
    }

    for (int y = 0; y < 32; y++)
    {
        int srcY = 31 - y;

        for (int x = 0; x < 32; x++)
        {
            if (iconbitmap[srcY * 32 + x] == 0x00)
            {
                continue;
            }

            DWORD color = iconbitmap[srcY * 32 + x];
            setpixel(posX + x, posY + y, color);
        }
    }
}

// BLUR CONFIG
void blurregion(DWORD* src, DWORD* dst, int x0, int y0, int w, int h, int radius)
{
    for (int y = y0; y < y0 + h; y++)
    {
        for (int x = x0; x < x0 + w; x++)
        {
            int r = 0, g = 0, b = 0, count = 0;

            for (int ky = -radius; ky <= radius; ky++)
            {
                for (int kx = -radius; kx <= radius; kx++)
                {
                    int px = x + kx;
                    int py = y + ky;

                    if (px < 0 || py < 0 || px >= WSCREEN || py >= HSCREEN)
                        continue;

                    DWORD c = src[py * WSCREEN + px];

                    r += (c >> 16) & 0xFF;
                    g += (c >> 8) & 0xFF;
                    b += c & 0xFF;
                    count++;
                }
            }

            r /= count;
            g /= count;
            b /= count;

            dst[y * WSCREEN + x] =
                0xFF000000 |
                (r << 16) |
                (g << 8) |
                b;
        }
    }
}

void messagebox(WINDOW msgbox, ICON icon, char* text, int nextWindow)
{
    windowss(msgbox, nextWindow);

    drawmessageicon(icon, msgbox.x + 25, msgbox.y + 35);

    int saveX = GetCursorX();
    int saveY = GetCursorY();

    SetCursorX(msgbox.x + 68);
    SetCursorY(msgbox.y + 40);

    Print(text, 0x0F);

    SetCursorX(saveX);
    SetCursorY(saveY);

    getwindow(msgbox);
}
