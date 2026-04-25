#include "variables.h"
#include "ports.h"
#include "text.h"

#include "board.h"

DWORD pciconfigreadword(BYTE bus, BYTE slot, BYTE func, BYTE offset)
{
    DWORD address;
    DWORD lbus = (DWORD)bus;
    DWORD lslot = (DWORD)slot;
    DWORD lfunc = (DWORD)func;
    DWORD tmp = 0;

    address = (DWORD)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((DWORD)0x80000000));

    outl(PCICONFIGADDRESS, address);

    tmp = inl(PCICONFIGDATA);

    return tmp;
}

void showdevices()
{
    for (BYTE device = 0; device < 32; device++) 
    {
        for (BYTE func = 0; func < 8; func++) 
        {
            DWORD data = pciconfigreadword(0, device, func, 0);
            WORD vendorid = (uint16_t)(data & 0xFFFF);
            WORD deviceid = (uint16_t)(data >> 16);

            if (vendorid != 0xFFFF) 
            {
                Debug("Device found at ", 2);
                printint(device, 0xFFFFFFFF);
                Print(", ", 0xFFFFFFFF);
                printint(func, 0xFFFFFFFF);
                Print(": Vendor ID = ", 0xFFFFFFFF);
                printhex(vendorid, 0xFFFFFFFF);
                Print(", Device ID = ", 0xFFFFFFFF);
                printhex(deviceid, 0xFFFFFFFF);
                Print("\n", 0x0F);
            }
        }
    }
}