#ifndef MEMH
#define MEMH
typedef unsigned char  uint8_t,  BYTE, UCHAR;
typedef unsigned short uint16_t, WORD, UWORD;
typedef unsigned int   uint32_t, DWORD, UINT;
typedef unsigned long  ULONG;
typedef unsigned short USHORT;
typedef unsigned int size_t;
typedef unsigned long long uint64_t, QWORD, ULLONG;

typedef char   CHAR;
typedef short  SHORT;
typedef int    INT;
typedef float  FLOAT;
typedef double DOUBLE;
typedef long   LONG;

typedef BYTE*  LPBYTE;
typedef WORD*  LPWORD;
typedef DWORD* LPDWORD;

typedef const char* STRING;

#define NULL  ((void*)0)

#define TRUE  1
#define true  1

#define FALSE 0
#define false 0

void* memoryset(void* dts, char val, DWORD num);
void* memorycapacity(void *dest, const void *src, DWORD n);
int strcmp(const char *str1, const char *str2);
char* strncpy(char *dest, const char *src, DWORD n);
int strncmp(const char *str1, const char *str2, DWORD n);
DWORD strlen(const char *str);
char* strcat(char* dest, const char* src);
char* strcpy(char* dest, const char* src);
void* memorymove(void* dest, const void* src, DWORD n);
char* strrchr(const char* str, int c);
int memorycmp(const void *s1, const void *s2, DWORD n);

#endif