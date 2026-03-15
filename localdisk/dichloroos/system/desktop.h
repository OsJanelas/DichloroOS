//DESKTOP
#define MAXITEMS 5

typedef struct 
{
    char name[16];
    int type;
    int x, y;
} desktopItem;

desktopItem* Getclickeditem(int x, int y);
void setdesktopindex(char* name, int type, int x, int y);
void desktopevents(int x, int y, int pressed);