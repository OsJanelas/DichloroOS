void drawchar(BYTE* bitmap, DWORD color);
void Print(const char* str, DWORD color);
void Debug(const char* str, int debug);
void inttomsg(int value, char* buffer);
void printint(int value, DWORD color);
void printhex(int value, DWORD color);
void printout(char letter, DWORD color);

int atoi(const char* str);
int GetCursorX();
int GetCursorY();
void SetCursorX(int x);
void SetCursorY(int y);