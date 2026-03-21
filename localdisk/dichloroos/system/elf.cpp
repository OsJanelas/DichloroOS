#include "include/variables.h"
#include "text.h"
#include "memory.h"
#include "virtmem.h"
#include "memfilesys.h"

#include "elf.h"

int loadelf(void* elfData) 
{
    DWORD relocationOffs = 0;

    Print("\n", 0x00);
    debug("Loading ELF...\n", 0x02);

    ELF32_Header* elfheader = (ELF32_Header*)elfData;

    if (*(DWORD*)elfHeader->e_ident != ELFMAGIC) 
    {
        debug("Invalid file\n", 0x01);
        return -1;
    }

    debug("Valid file\n", 0x00);

    if (elfHeader->e_machine != ELFARCH) 
    {
        debug("Unsupported Architecture\n", 0x01);
        return -1;
    }

    debug("Valid Architecture\n", 0x00);

    ELF32_ProgramHeader* programHeader = (ELF32_ProgramHeader*)((BYTE*)elfData + elfHeader->e_phoff);

    for (WORD i = 0; i < elfHeader->e_phnum; i++) 
    {
        if (programHeader[i].p_type != PTLOAD) 
        {
            continue;
        }

        char* dest = (char*) programHeader[i].p_vaddr;
        char* src = elfData + programHeader[i].p_offset;
        DWORD filesz = programHeader[i].p_filesz;
        DWORD memsz = programHeader[i].p_memsz;

        allocatevirtualmemory((DWORD)dest, memsz, PROT_READ | PROT_WRITE | PROT_EXEC, 1);

        if (filesz > 0) 
        {
            memmove(dest, src, filesz);
        }

        if (memsz > filesz) 
        {
            memset(dest + filesz, 0, memsz - filesz);
        }
    }

    DWORD relocatedEntry = elfHeader->e_entry + relocationOffs;

    debug("ELF Entry Point: ", 0x02);
    printhex(relocatedEntry, 0xFFFFFFFF);
    Print("\n", 0x00);

    void (*entryPoint)(void) = (void (*)(void))(relocatedEntry);
    entryPoint();

    return 0;
}

void exeelf(void* elf)
{
    if (loadeLF(elf))
    {
        Debug("ERROR: FAILED TO LOAD ELF FILE\n", 0x01);
        return;
    }
}
