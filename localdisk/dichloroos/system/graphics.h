#define WSCREEN 760
#define HSCREEN 600

void initgraphics(LPWORD mbinfo);
DWORD getframebuffer();
DWORD getpitch();
void setpixel(int x, int y, DWORD color);
DWORD getpixel(int x, int y);
void clearscreen(void);
void drawpixel(int x, int y, int w, int h, DWORD color);