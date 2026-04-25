#include "variables.h"
#include "ports.h"
#include "text.h"
#include "disk.h"

void waitforready(WORD base)
{
    while (1)
    {
        BYTE status = inw(base + 7);

        if (status & IDESTATUSREADY)
        {
            break;
        }
    }
}

DWORD getdisksize(WORD base)
{
    outw(base + 6, 0xA0);
    outb(base + 7, 0xEC);

    waitforready(base);

    WORD data[256];

    for (int i = 0; i < 256; i++)
    {
        data[i] = inw(base);
    }

    DWORD sectors = ((DWORD)data[61] << 16) | data[60];

    return sectors;
}

DWORD getfloppysize(void)
{
    BYTE status = inw(FLOPPYSTATUSPORT);

    if (status & FLOPPYREADY)
    {
        return 1474560;
    }

    return 0;
}

void alldisks(void)
{
    DWORD diskcapacity;

    diskcapacity = getdisksize(IDEPRIMARYCMDPORT);

    if(diskcapacity > 0)
    {
        Debug("Disk 0: ", 0x02);
        Print("Capacity: ", 0xFFFFFFFF);
        printhex(diskcapacity, 0xFFFFFFFF);
        Print("Sectors\n", 0xFFFFFFFF);
    }
    else
    {
        Debug("ERROR: Don't found disks on primmary IDE channel\n", 0x01);
    }

    DWORD floppycapacity = getfloppysize();
    
    if (floppycapacity >  0)
    {
        Debug("Floppy: ", 0x02);
        Print("Capacity: ", 0xFFFFFFFF);
        printint(floppycapacity, 0xFFFFFFFF);
        Print("Bytes\n", 0xFFFFFFFF);
    }
    else
    {
        Debug("ERROR: Don't found floppy\n", 0x01);
    }
}