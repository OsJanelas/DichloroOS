
#ifndef _ALLOC_H
#define _ALLOC_H

#include "variables.h"

#define MEMPOOL 0x100000
#define BLOCK_SIZE sizeof(MemoryBlock)

typedef struct MemoryBlock {
    struct MemoryBlock* next;
    DWORD size;
    BYTE used;
} MemoryBlock;

typedef struct {
    MemoryBlock* freeBlocks;
    BYTE memory[MEMPOOL];
} MemoryManager;


void initmemory();
void* allocatememory(DWORD);
void freememory(void* ptr);
void* callocatememory(int count, int size);

#endif