#define MAXFILENAME 0x0F
#define MAXFILES 0x0F
#define MAXSUBDIR 0x0F
#define FSADRESS 0xC00000

#define PERM_R 0x04
#define PERM_W 0x02
#define PERM_X 0x01

typedef struct Directory Directory;

typedef struct
{
    char filename[MAXFILENAME];
    DWORD size;
    DWORD dataoffset;
    BYTE permissions;
} FileHeader;

struct Directory
{
    char name[MAXFILENAME];
    FileHeader files[MAXFILES];

    Directory* subdirs[MAXSUBDIR];
    Directory* parent;

    DWORD nextfreeblock;
};

typedef struct
{
    Directory root;
    Directory* currentdir;
} filesystem;

void initfilesystem();
int createfile(const char* filename, const LPBYTE data, DWORD size, BYTE permissions);
int readfile(const char* filename, LPBYTE buffer, LPDWORD size);
int deletefile(const char* filename);
int findfile(const char* filename);
int fileinfo(const char* filename);
int renamefile(const char* oldfilename, const char* newfilename);
void listfiles();
int makedir(const char* dir);
int deletedir(const char* dirname);
int changedir(const char* dirname);
void listdirs();

void printcurrentdir();

Directory* getrootdir();