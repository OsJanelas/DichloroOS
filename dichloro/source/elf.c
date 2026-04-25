#include "variables.h"
#include "text.h"
#include "memory.h"
#include "virtmem.h"
#include "memfilesys.h"
#include "filesystem.h"
#include "allocate.h"

#include "elf.h"

void* loadelf(void* elfData) 
{
    DWORD relocationOffs = 0;

    Print("\n", 0x00);
    Debug("Loading ELF...\n", 0x02);

    ELF32_Header* elfheader = (ELF32_Header*)elfData;

    if (*(DWORD*)elfheader->e_ident != ELFMAGIC) 
    {
        Debug("Invalid file\n", 0x01);
        return NULL;
    }

    Debug("Valid file\n", 0x00);

    if (elfheader->e_machine != ELFARCH) 
    {
        Debug("Unsupported Architecture\n", 0x01);
        return NULL;
    }

    Debug("Valid Architecture\n", 0x00);

    ELF32_ProgramHeader* programHeader = (ELF32_ProgramHeader*)((BYTE*)elfData + elfheader->e_phoff);

    for (WORD i = 0; i < elfheader->e_phnum; i++) 
    {
        if (programHeader[i].p_type != PTLOAD) 
        {
            continue;
        }

        char* dest = (char*) programHeader[i].p_vaddr;
        char* src = elfData + programHeader[i].p_offset;
        DWORD filesz = programHeader[i].p_filesz;
        DWORD memorysize = programHeader[i].p_memsz;

        allocvirtmem((DWORD)dest, memorysize, PROTREAD | PROTWRITE | PROTEXEC, 1);

        if (filesz > 0) 
        {
            memorymove(dest, src, filesz);
        }

        if (memorysize > filesz) 
        {
            memoryset(dest + filesz, 0, memorysize - filesz);
        }
    }

    DWORD relocatedEntry = elfheader->e_entry + relocationOffs;

    Debug("ELF Entry Point: ", 0x02);
    printhex(relocatedEntry, 0xFFFFFFFF);
    Print("\n", 0x00);

    return (void*)relocatedEntry;
}

void exeelf(void* elf)
{
    void (*entry)() = loadelf(elf);
    if (!entry)
    {
        Debug("ERROR: FAILED TO LOAD ELF FILE\n", 0x01);
        return;
    }
    entry();
}