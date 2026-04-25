#include "variables.h"
#include "text.h"

#include "processor.h"

char vendor[13];
char cpustring[49];

void getvendor(char* vendor)
{
    int ebx, edx, ecx;

    asm volatile 
    (
        "xor %%eax, %%eax\n\t"
        "cpuid\n\t"
        : "=b" (ebx),
          "=d" (edx),
          "=c" (ecx)
        :
        : "eax"
    );

    *(int *)&vendor[0] = ebx;
    *(int *)&vendor[4] = edx;
    *(int *)&vendor[8] = ecx;
    vendor[12] = 0;
}

void cpuname(char* name)
{
    DWORD eax, ebx, ecx, edx;

    for (int i = 0; i < 3; i++) 
    {
        eax = 0x80000002 + i;

        asm volatile 
        (
            "cpuid"
            : "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
            : "a" (eax)
        );

        *(unsigned int*)(name + i * 16 + 0) = eax;
        *(unsigned int*)(name + i * 16 + 4) = ebx;
        *(unsigned int*)(name + i * 16 + 8) = ecx;
        *(unsigned int*)(name + i * 16 + 12) = edx;
    }

    name[48] = '\0';
}

void showcpuname()
{
    Print(cpustring, 0xFFFFFFFF);
    Print("\n", 0x00);
}

void showcpuinfo()
{
    cpuname(cpustring);
    getvendor(vendor);
   
    Debug("CPU Vendor: ", 2);
    Print(vendor, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("CPU: ", 2);
    Print(cpustring, 0xFFFFFFFF);
    Print("\n", 0x00);
}
