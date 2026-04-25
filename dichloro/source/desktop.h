#define MAXITEMS 5

typedef struct
{
    char name[16];
    int type;
    int x, y;
} item;

item* getclickeditem(int x, int y);
void setdektopindex(char* name,  int type, int x, int y);