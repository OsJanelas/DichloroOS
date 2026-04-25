void initgraphics(LPDWORD mbinfo);
DWORD getframebuff();
DWORD getpitch();
void setpixel(int x, int y, DWORD color);
DWORD getpixel(int x, int y);
void clearscren(void);
void drawpixel(int x, int y, int w, int h, DWORD color);