typedef struct
{
    BYTE e_ident[16];
    WORD e_type;
    WORD e_machine;
    DWORD e_version;
    DWORD e_entry;
    DWORD e_phoff;
    DWORD e_shoff;
    DWORD e_flags;
    WORD e_ehsize;
    WORD e_phentsize;
    WORD e_phnum;
    WORD e_shentsize;
    WORD e_shnum;
    WORD e_shstrndx;
} Elf32_Header;

typedef struct
{
    DWORD p_type;
    DWORD p_offset;
    DWORD p_vaddr;
    DWORD p_paddr;
    DWORD p_filesz;
    DWORD p_memsz;
    DWORD p_flags;
    DWORD p_align;
} Elf32_ProgramHeader;

#define ELFMAGIC 0x464C457F
#define ELFARCH 0x03

#define PTNULL 0x00
#define PTLOAD 0x01

int loadelf(void* elfdata);
int exeelf(void* elf);