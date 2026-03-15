//GUI
typedef enum
{
    ICON_NONE,
    ICON_ERROR,
    ICON_WARNING,
    ICON_INFO,
    ICON_QUESTION,
    ICON_USER1,
    ICON_TERMINAL
} ICON;

typedef enum
{
    ICON_TEXT,
    ICON_DIR,
    ICON_EXE,
    ICON_DEFAULT,
    ICON_USER2
} EXTICON;

typedef struct
{
    const BYTE* title;
    DWORD color;

    int x, y;
    int w, h;

    int ismaximized;
} WINDOW;

typedef struct GUIevent
{
    int type;
    int mouseX, mouseY;
    int keycode;
    int windowID; 
} GUIevent;

typedef struct graphicsbuffer
{
    int width, heights;
    BYTE* data;
} graphicsbuffer;