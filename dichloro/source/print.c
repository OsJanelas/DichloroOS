#include "variables.h"
#include "video.h"
#include "graphics.h"
#include "font.h"
#include "text.h"

int cursorX = 0;
int cursorY = 0;

void printint(int value, DWORD color)
{
    char buffer[11];
    inttomsg(value, buffer);
    Print(buffer, color);
}

int atoi(const char* str)
{
    int result = 0;
    int sign = 1;
    int i = 0;

    while (str[i] == ' ') 
    {
        i++;
    }

    if (str[i] == '-') 
    {
        sign = -1;
        i++;
    } 
    else if (str[i] == '+') 
    {
        i++;
    }

    while (str[i] >= '0' && str[i] <= '9')
     {
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

void drawchar(BYTE* bitmap, DWORD color)
{
    int i = 0;

    for (int y = 0; y < HFONT; y++)
    {
        for (int x = WFONT - 1; x >= 0; x--)
        {
            if (bitmap[y] & (1 << x))
            {
                setpixel(i + cursorX, y + cursorY, color);
            }

            i++;
        }

        i = 0;
    }

    cursorX += 8;

    if (cursorX >= WSCREEN)
    {
        cursorX = 0;
        cursorY += 16;
    }
}

void Debug(const char* str, int debug)
{
    
    switch (debug)
    {
        case 0:
            Print("[ Good ] ", 0xFF00FF00);    
            break;
        case 1:
            Print("[Failed] ", 0xFFFF0000);
            break;
        case 2:
            Print("[ Info ] ", 0xFF00FFFF);
            break;
    }

    Print(str, 0xFFFFFFFF);
}

void Print(const char* str, DWORD color)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n')
        {
            cursorX = 0;
            cursorY += HFONT;

            continue;
        }

        if (str[i] == '\b')
        {
            if (cursorX != 0 && cursorY != 0)
            {
                cursorX -= 24;
                Print(" ", 0x00);
            }
        }

        if (str[i] == '\f')
        {
            drawchar(font + 128 * HFONT, color);

            continue;
        }

        drawchar(font + str[i] * HFONT, color);
    }
}

void inttomsg(int value, char* buffer)
{
    char temp[11];
    int i = 0;
    int isnegative = 0;

    if (value < 0) 
    {
        isnegative = 1;
        value = -value;
    }

    do 
    {
        temp[i++] = (value % 10) + '0';
        value /= 10;
    } while (value > 0);

    if (isnegative) 
    {
        temp[i++] = '-';
    }

    temp[i] = '\0';

    int j;

    for (j = 0; j < i; j++) 
    {
        buffer[j] = temp[i - j - 1];
    }

    buffer[j] = '\0';
}

void printhex(int value, DWORD color)
{
    char buffer[9];

    Print("0x", color);

    for (int i = 7; i >= 0; i--) 
    {
        buffer[i] = "0123456789ABCDEF"[value & 0xF];
        value >>= 4;
    }

    buffer[8] = '\0';

    Print(buffer, color);
}

void printout(char letter, DWORD color)
{
    drawchar(font + letter * HFONT, color);
}

//--------------------------------------------------------------

int GetCursorX()
{
    return cursorX;
}

int GetCursorY()
{
    return cursorY;
}

void SetCursorX(int x)
{
    cursorX = x;
}

void SetCursorY(int y)
{
    cursorY = y;
}