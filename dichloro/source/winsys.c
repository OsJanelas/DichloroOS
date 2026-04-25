#include "variables.h"
#include "video.h"
#include "graphics.h"
#include "text.h"
#include "allocate.h"
#include "userspace.h"
#include "memory.h"

#include "winsys.h"
#include "gui.h"

int startwindowXY = 80;

WINDOW* windowslist  = 0;
WINDOW* activewindow = 0;

WINDOW winmgr[100];
int currentWindowID = 0;
int totalWindows = 0;

void getwindow(WINDOW window)
{
    winmgr[currentWindowID] = window;

    winmgr[currentWindowID].orgX = window.x;
    winmgr[currentWindowID].orgY = window.y;
    winmgr[currentWindowID].orgW = window.w;
    winmgr[currentWindowID].orgH = window.h;

    currentWindowID++;
    totalWindows++;
}

WINDOW* createwindow(int w, int h, DWORD color, char* title)
{
    WINDOW* win = (WINDOW*)allocatememory(sizeof(WINDOW));

    if (!win) return NULL;

    win->x = startwindowXY;
    win->y = startwindowXY;

    win->w = w;
    win->h = h;
    win->color = color;
    win->title = title;

    win->next = NULL;
    win->prev = NULL;

    addwindow(win);

    startwindowXY += 40;

    if (startwindowXY > 200)
    {
        startwindowXY = 80;
    }

    activewindow = win;

    showallwindows();

    return win;
}

void bringtofront(WINDOW* win)
{
    if (win == windowslist) return;

    if (win->prev)
        win->prev->next = win->next;

    if (win->next)
        win->next->prev = win->prev;

    win->next = windowslist;
    win->prev = NULL;

    if (windowslist)
        windowslist->prev = win;

    windowslist = win;
}

void drawrect(int x, int y, int w, int h, int radius, DWORD color)
{
    int right = x + w;
    int bottom = y + h;

    DWORD pixelcolor = color;

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x + radius; j < right - radius; j++) 
        {
            setpixel(j, i, pixelcolor);
        }
    }

    for (int i = x + radius; i < right - radius; i++) 
    {
        for (int j = y; j < y + radius; j++) 
        {
            setpixel(i, j, pixelcolor);
        }

        for (int j = bottom - radius; j < bottom; j++) 
        {
            setpixel(i, j, pixelcolor);
        }
    }

    for (int i = y + radius; i < bottom - radius; i++) 
    {
        for (int j = x; j < x + radius; j++) 
        {
            setpixel(j, i, pixelcolor);
        }
        for (int j = right - radius; j < right; j++) 
        {
            setpixel(j, i, pixelcolor);
        }
    }

    for (int dx = -radius; dx <= radius; dx++) 
    {
        for (int dy = -radius; dy <= radius; dy++) 
        {
            if (dx * dx + dy * dy <= radius * radius) 
            {
                setpixel(x + radius + dx, y + radius + dy, pixelcolor);

                setpixel(right - radius - 1 + dx, y + radius + dy, pixelcolor);

                setpixel(x + radius + dx, bottom - radius - 1 + dy, pixelcolor);

                setpixel(right - radius - 1 + dx, bottom - radius - 1 + dy, pixelcolor);
            }
        }
    }
}

void windowss(WINDOW window, int nextWindow)
{
    drawrect(window.x, window.y, window.w, window.h, 5, window.color);

    if (window.title)
    {
        SetCursorX(window.x + 10);
        SetCursorY(window.y +  4);

        Print(window.title, 0xFFFFFFFF);
    }

    if (nextWindow)
    {
        startwindowXY += 40;

        if (startwindowXY > 200)
        {
            startwindowXY = 80;
        }
    }
}

void showallwindows()
{
    WINDOW* current = windowslist;

    while (current && current->next)
    {
        current = current->next;
    }

    while (current)
    {
        windowss(*current, 0);
        current = current->prev;
    }
}

void addwindow(WINDOW* window)
{
    window->next = windowslist;
    window->prev = 0;

    if (windowslist)
    {
        windowslist->prev = window;
    }
    windowslist = window;
}

void dragging(int mouseX, int mouseY, int pressed)
{
    static int isdragging = 0;
    static int offsetX = 0;
    static int offsetY = 0;
    static WINDOW* dragwindow = NULL;
    static int lastpressed = 0;

    if (pressed && !lastpressed)
    {
        WINDOW* win = getat(mouseX, mouseY);

        if (win)
        {
            if (mouseY >= win->y && mouseY <= win->y + 20)
            {
                dragwindow = win;
                isdragging = 1;

                offsetX = mouseX - win->x;
                offsetY = mouseY - win->y;

                bringtofront(win);
                activewindow = win;
            }
        }
    }

    if (pressed && isdragging && dragwindow)
    {
        dragwindow->x = mouseX - offsetX;
        dragwindow->y = mouseY - offsetY;

        if (dragwindow->x < 0) dragwindow->x = 0;
        if (dragwindow->y < 0) dragwindow->y = 0;
        if (dragwindow->x + dragwindow->w > WSCREEN)
            dragwindow->x = WSCREEN - dragwindow->w;
        if (dragwindow->y + dragwindow->h > HSCREEN)
            dragwindow->y = HSCREEN - dragwindow->h;

        showallwindows();
    }

    if (!pressed)
    {
        isdragging = 0;
        dragwindow = NULL;
    }

    lastpressed = pressed;
}

WINDOW* getat(int x, int y)
{
    WINDOW* current = windowslist;

    while (current)
    {
        if (x >= current->x && x <= current->x + current->w && y >= current->y && y <= current->y + current->h)
        {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void handleclick(int x, int y, int pressed)
{
    static int lastpressed = 0;

    if (pressed && !lastpressed)
    {
        WINDOW* win = getat(x, y);

        if (win)
        {
            bringtofront(win);
            activewindow = win;
        }
        else
        {
            activewindow = NULL;

            if (x > WSCREEN - 100 && y > HSCREEN - 100)
            {
                createwindow(200, 150, 0xFFA1A1A1, "Dichloro Window");
            }
        }

        showallwindows();
    }

    lastpressed = pressed;
}

void maximizewindow(int x, int y, int pressed)
{
    const int buttonSize = 10;

    for (int i = 0; i < totalWindows; i++)
    {
        WINDOW* win = &winmgr[i];

        int buttonXStart = win->x + win->w - (2 * buttonSize) - 20;
        int buttonXEnd = buttonXStart + buttonSize;
        int buttonYStart = win->y + 5;
        int buttonYEnd = buttonYStart + buttonSize;

        if (pressed && x >= buttonXStart && x < buttonXEnd && y >= buttonYStart && y < buttonYEnd)
        {
            if (win->isMaximized % 2 == 0)
            {
                //SaveTotalArea();

                win->x = 0;
                win->y = 0;
                win->w = WSCREEN;
                win->h = HSCREEN - 32;

                win->isMaximized++;

                windowss(*win, 0x00);
            }
            else
            {
                win->x = getstartwindowxy();
                win->y = getstartwindowy();
                win->w = 320;
                win->h = 200;

                win->isMaximized++;

                wallpaper();

                windowss(*win, 0x00);
            }

            if (strcmp(win->title, "Run") == 0x00)
            {
                //StartShell(*win);
            }
        }
    }
}

int getstartwindowxy()
{
    return startwindowXY;
}

int getstartwindowy()
{
    return startwindowXY;
}