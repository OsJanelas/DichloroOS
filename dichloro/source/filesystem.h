typedef struct
{
    char filename[MAXFILENAME];
    DWORD size;
    DWORD dataoffset;
    BYTE permissions;
} fileheader;

struct directory
{
    char name[MAXFILENAME];
    fileheader files[MAXFILES];

    directory* subdirs[MAXSUBDIR];
    directory* parent;

    DWORD nextfreeblock;
};

typedef struct
{
    directory root;
    directory* currentdir;
} filesystem;

void initfs();
int createfile(const char* filename, const LPBYTE data, DWORD size, BYTE permissions);
int readfile(const char* filename, LPBYTE buffer, LPDWORD size);
int deletefile(const char* filename);
int findfile(const char* filename);
int fileinfo(const char* filename);
int renamefile(const char* oldfilename, const char* newfilename);
void listfiles();

int createdir(const char* dir);
int deletedir(const char* dirname);
int changedir(const char* dirname);
void listdirs();

void showcurrentfolder();
int readchunk(const char* filename, BYTE* buffer, DWORD offset, DWORD bytestoread);

directory* getrootdir();