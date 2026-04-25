#define PAGESIZE 4096
#define TOTALPAGES 1024
#define VIRTUALMEMORYSIZE (PAGESIZE * TOTALPAGES)

#define PROTREAD  0x1
#define PROTWRITE 0x2
#define PROTEXEC  0x4
#define PROTUSER  0x8

typedef struct pagetableentry 
{
    DWORD physicaladdress;
    BYTE present;
    BYTE writable;
    BYTE user;
} pagetableentry;

typedef struct virtmemmanager 
{
    pagetableentry pagetable[TOTALPAGES];
    BYTE physicalmemory[TOTALPAGES * PAGESIZE];
} virtmemmanager;

void startvirtmem();
int mappage(DWORD virtualAddress, DWORD physicalAddress, BYTE writable, BYTE user);
void unmappage(DWORD virtualAddress);
DWORD translate(DWORD virtualAddress);
void* allocvirtmem(DWORD virtualAddress, DWORD size, BYTE writable, BYTE user);
void freevirtmem(DWORD virtualAddress, DWORD size);
int setprotection(DWORD virtualAddress, DWORD size, BYTE protectionFlags);