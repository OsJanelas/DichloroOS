void initmouse();
void mousehandler(struct InterruptRegisters *r);
void mousewait();
void resetpreviouscursor();
void savearea();
void drawcursor(DWORD color);
void getstate(int* x, int* y, int* pressed);