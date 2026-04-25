typedef struct WINDOW
{
    const char* title;
    DWORD color;

    int x, y;
    int w, h;

    int orgX, orgY, orgW, orgH;

    int dragOffsetX;
    int dragOffsetY;

    struct WINDOW* next;
    struct WINDOW* prev;

    int isMaximized;
} WINDOW;

void windowss(WINDOW window, int nextWindow);
void addwindow(WINDOW* window);
void bringtofront(WINDOW* win);
void handleclick(int x, int y, int pressed);
void showallwindows();
WINDOW* getat(int x, int y);
WINDOW* createwindow(int w, int h, DWORD color, char* title);
void drawrect(int x, int y, int w, int h, int radius, DWORD color);
void dragging(int mouseX, int mouseY, int pressed);
void maximizewindow(int x, int y, int pressed);