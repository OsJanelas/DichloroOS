#define PAGESIZE 4096
#define TOTALPAGES 1024
#define VIRTUALMEMORYSIZE (PAGESIZE * TOTALPAGES)

#define PROTHEAD 0x1
#define PROTWRITE 0x2
#define PROTEXEC 0x4
#define PROTUSER 0x8

typedef struct pagetableentry
{
    DWORD pyshicaladress
    BYTE present;
    BYTE writable;
    BYTE user;
} pagetableentry;

typedef struct virtualmemorymanager
{
    pagetableentry pagetable[TOTALPAGES];
    BYTE pyshicalmemory[TOTALPAGES * PAGESIZE];
} virtualmemorymanager;

void initvmm;
int mappage(DWORD virtualadress, DWORD pyshicaladress, BYTE writable, BYTE user);
void unmappage(DWORD virtualadress);
void transadress(DWORD virtualadress);
void* allocatevirtualmemory(DWORD virtualaddress, DWORD size, BYTE writable, BYTE user);
void freevirtualmemory(DWORD virtualaddress, DWORD size);
int setmemoryprotection(DWORD virtualaddress, DWORD size, BYTE protectionflags);