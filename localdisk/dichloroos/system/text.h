void drawletter(BYTE* bitmap, DWORD color);
void Print(const char* str, DWORD color);
void debug(const CHAR* str, int debug);
void inttomsg(int value, DWORD color);
void printint(int value, DWORD color);
void printhex(int value, DWORD color);

int atoi(const char* str);
int getcursorX;
int getcursorY;
void setcursorX(int x);
void setcursorY(int y);