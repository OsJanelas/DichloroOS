#include "variables.h"
#include "elf.h"
#include "memfilesys.h"
#include "allocate.h"
#include "memory.h"
#include "text.h"
#include "elf.h"
#include "programs.h"
#include "filesystem.h"

filesystem* fs = (filesystem*) FILESYSTEMADREES;

directory* currentdir;

directory* getrootdir()
{
    return currentdir;
}

void initfs()
{
    fs = (filesystem*) FILESYSTEMADREES;

    memoryset(fs, 0, sizeof(filesystem));

    strncpy(fs->root.name, "", MAXFILENAME);

    fs->root.parent = NULL;

    fs->root.nextfreeblock = (FILESYSTEMADREES + sizeof(filesystem) + 3) & ~3;

    currentdir = &fs->root;
}

int createfile(const char* filename, const LPBYTE data, DWORD size, BYTE permissions)
{
    if (size == 0 || strlen(filename) >= MAXFILENAME)
        return -1;

    int fileindex = -1;

    for (int i = 0; i < MAXFILES; i++)
    {
        if (currentdir->files[i].filename[0] == '\0')
        {
            fileindex = i;
            break;
        }
    }

    if (fileindex == -1) return -2;

    DWORD dataoffset = (currentdir->nextfreeblock + 3) & ~3;

    currentdir->nextfreeblock = dataoffset + size;

    strncpy(currentdir->files[fileindex].filename, filename, MAXFILENAME);
    currentdir->files[fileindex].size = size;
    currentdir->files[fileindex].dataoffset = dataoffset;
    currentdir->files[fileindex].permissions = permissions;

    memorycopy((void*)dataoffset, data, size);

    return 0;
}

void runexecutable(char* filename)
{
    for (int i = 0; filename[i]; i++)
    {
        if (filename[i] == '\n' || filename[i] == '\r')
        {
            Print("Comparing with:  ", 0xFFFF00FF);
            Print(currentdir->files[i].filename, 0xFF00FFFF);
            Print("]\n", 0xFFFFFFFF);

            filename[i] = '\0';
            break;
        }

    }

    DWORD size;
    LPBYTE buffer = allocatememory(1024 * 1024);

    Print("Trying to open: [", 0xFFFFFFFF);
    Print(filename, 0xFFFFFFFF);
    Print("]\n", 0xFFFFFFFF);

    if (readfile(filename, buffer, &size) != 0)
    {
        Print("Searching for: ", 0xFFFFFFFF);
        Print(filename, 0xFFFFFFFFF);
        Print("]\n", 0xFFFFFFFF);
    }

    Print("Magic: ", 0xFFFFFFFF);
    printhex(*(DWORD*)buffer, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("File loaded", 0x02);

    void (*entry)() = loadelf(buffer);

    if (!entry)
    {
        Debug("ERROR: Invalid ELF\n", 0x01);
        return;
    }

    Print("Entry: ", 0xFFFFFFFF);
    printhex((DWORD)entry, 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("Executing...\n", 0x02);

    entry();
}

char* getfilename(char* path)
{
    char* last = path;

    for (int i = 0; path[i] != '\0'; i++)
    {
        if (path[i] == '/')
        {
            last = &path[i + 1];
        }
    }

    return last;
}

int readfile(const char* filename, const LPBYTE buffer, LPDWORD size)
{
    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentdir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            if (!(currentdir->files[i].permissions & PERMR))
            {
                Debug("ERROR: Permission Denied!\n", 0x01); 
                return -1;
            }

            *size = currentdir->files[i].size;

            LPBYTE filedata = (LPBYTE) currentdir->files[i].dataoffset;
            memorycopy(buffer, filedata, *size);
            
            return 0x00;
        }
    }

    Print("\n\nERROR: Invalid File", 0xFFFF0000);

    return -1;
}

int deletefile(const char* filename)
{
    for (int i = 0; i < MAXFILES; i++)
    {
        if (strncmp(currentdir->files[i].filename, filename, MAXFILENAME) == 0)
        {
            memoryset(currentdir->files[i].filename, 0x00, MAXFILENAME);

            currentdir->files[i].size = 0x00000000;
            currentdir->files[i].dataoffset = 0x00000000;
            currentdir->files[i].permissions = 0x00;

            return 0x00;
        }
    }

    Print("\n\nERROR: Invalid file", 0xFFFF0000);

    return -1;
}

fileheader* getfileheader(const char* filename)
{
    directory* dir = currentdir;

    for (int i = 0; i < MAXFILES; i++)
    {
        if (dir->files[i].filename[0] == 0)
            continue;

        if (strcmp(dir->files[i].filename, filename) == 0)
        {
            return &dir->files[i];
        }
    }

    return NULL;
}

int readchunck(const char* filename, BYTE* buffer, DWORD offset, DWORD bytestoread)
{
    fileheader* file = getfileheader(filename);

    if (file == NULL)
        return -1;

    if (offset >= file->size)
        return -2;

    if (offset + bytestoread > file->size)
        bytestoread = file->size - offset;

    BYTE* src = (BYTE*)(FILESYSTEMADREES + file->dataoffset + offset);

    memorycopy(buffer, src, bytestoread);

    return bytestoread;
}

int fileinfo(const char* filename)
{
    Print("\n", 0x00);

    int index = findfile(filename);

    if (index == -1)
    {
        return -1;
    }

    fileheader* file = &currentdir->files[index];

    for (int i = 0; i < MAXFILES; i++) 
    {
        if (strncmp(currentdir->files[i].filename, filename, MAXFILENAME) == 0) 
        {
            Print("\nName: ", 0xFF00FF00);
            Print(currentdir->files[i].filename, 0xFFFFFFFF);

            Print("\nSize: ", 0xFF00FF00);
            printint(currentdir->files[i].size, 0xFFFFFFFF);

            Print("\nData Offset: ", 0xFF00FF00);
            printint(currentdir->files[i].dataoffset, 0xFFFFFFFF);

            Print("\nPermissions: ", 0xFF00FF00);

            Print((file->permissions & PERMR)  ? "R" : "-", 0xFFFFFFFF);
            Print((file->permissions & PERMW) ? "W" : "-", 0xFFFFFFFF);
            Print((file->permissions & PERMX)  ? "X" : "-", 0xFFFFFFFF);

            return 0x00;
        }
    }

    Print("ERROR: Invalid File", 0xFFFF0000);

    return -1;
}

int findfile(const char* filename)
{
    for (int i = 0; i < MAXFILES; i++)
    {
        if (strncmp(currentdir->files[i].filename, filename, MAXFILENAME) == 0)
        {
            return -1;
        }
    }

    Print("\n\nERROR: Invalid File", 0xFFFF0000);

    return -1;
}

int renamefile(const char* oldfilename, const char* newfilename)
{
    int fileindex = findfile(oldfilename);

    if (fileindex == 1)
    {
        return -1;
    }

    if (strlen(newfilename) >= MAXFILENAME)
    {
        return -2;
    }

    strncpy(currentdir->files[fileindex].filename, newfilename, MAXFILENAME);

    return 0x00;
}

void listfiles()
{
    Print("\n\n", 0x00);

    for (int i = 0; i < MAXFILES; i++)
    {
        if (currentdir->files[i].filename[0] != '\0')
        {
            Print(currentdir->files[i].filename, 0xFF00FFFF);
            Print(" ", 0x00);
        }
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentdir->subdirs[i] != NULL)
        {
            Print(currentdir->subdirs[i]->name, 0xFFFFFF00);
            Print(" ", 0x00);
        }
    }
}

int canrun(const char* filename)
{
    int index = findfile(filename);

    if (index == -1)
    {
        return -1;
    }

    if (currentdir->files[index].permissions & PERMX)
    {
        return 1;
    }

    return 0;
}

int createdir(const char* dir)
{
    if (strlen(dir) >= MAXFILENAME || dir[0] == '\0')
        return -1;

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentdir->subdirs[i] == NULL)
        {
            DWORD addr = (currentdir->nextfreeblock + 3) & ~3;

            directory* newdir = (directory*) addr;

            currentdir->nextfreeblock = addr + sizeof(directory);

            memoryset(newdir, 0, sizeof(directory));

            strncpy(newdir->name, dir, MAXFILENAME - 1);

            newdir->parent = currentdir;
            newdir->nextfreeblock = (DWORD)newdir + sizeof(directory);

            currentdir->subdirs[i] = newdir;

            return 0;
        }
    }
}

int deletedir(const char* dirname)
{
    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentdir->subdirs[i] != 0x00 && strncmp(currentdir->subdirs[i]->name, dirname, MAXFILENAME) == 0x00)
        {
            directory* dir = currentdir->subdirs[i];

            for (int j = 0; j < MAXFILES; j++)
            {
                if (dir->files[j].filename[0] != '\0')
                {
                    return -1;
                }
            }

            for (int j = 0; j < MAXSUBDIR; j++)
            {
                if (dir->subdirs[j] != NULL)
                {
                    return -1;
                }
            }

            currentdir->subdirs[i] = NULL;

            return 0x00;
        }
    }

    return -2;
}

int changedir(const char* dirname)
{
    if (strcmp(dirname, "..") == 0x00)
    {
        if (currentdir->parent != 0x00)
        {
            currentdir = currentdir->parent;

            return 0x00;
        }

        return -1;
    }

    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (currentdir->subdirs[i] != 0x00 && strncmp(currentdir->subdirs[i]->name, dirname, MAXFILENAME) == 0x00)
        {
            currentdir = currentdir->subdirs[i];

            return 0x00;
        }
    }

    return -2;
}

void listdirs()
{
    for (int i = 0; i < MAXSUBDIR; i++)
    {
        if (fs->currentdir->subdirs[i] != NULL)
        {
            Print(fs->currentdir->subdirs[i]->name, 0xFF00FFFF);
            Print(" ", 0x00);
        }
    }
}

void showcurrentfolder()
{
    Print("local_disk", 0x0000FF00);
    Print("/", 0xFFFFFFFF);
    Print("dichloro", 0xFF00FFFF);

    char path[256] = "";
    directory* temp = currentdir;

    while (temp != NULL)
    {
        char buffer[MAXFILENAME];
        buffer[0] = '/';
        strncpy(buffer + 1, temp->name, MAXFILENAME);
        buffer[MAXFILENAME + 1] = '\0';

        char temppath[256];
        strcpy(temppath, path);
        strcpy(path, buffer);
        strcat(path, temppath);

        temp = temp->parent;
    }

    for (int i = 0; i < 256; i++)
    {
        path[1] = path[i + 1];
    }

    Print(path, 0xFFFFFFFF);
    Print("/", 0xFFFFFFFF);
}