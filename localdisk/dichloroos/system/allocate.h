#ifndef _ALLOC_H
#define _ALLOC_H

#include "include/variables.h"

#define MEMPOOL 0x100000
#define BLOCKSIZE sizeof(memoryblock)

typedef struct memoryblock {
    struct memoryblock* next;
    DWORD size;
    BYTE used;
} memoryblock;

typedef struct {
    memoryblock* freeblocks;
    BYTE memory[MEMPOOL];
} memorymanager;


void initmemory();
void* allocatememory(DWORD size);
void freememory(void* ptr);
void* Callocatememory(int count, int size);

#endif