#ifndef MEMH
#define MEMH

void* memoryset(void* dts, char val, DWORD num);
void* memorycopy(void *dest, const void *src, DWORD n);
void* memorymove(void* dest, const void* src, DWORD n);
int memorycmp(const void *s1, const void *s2, DWORD n);

int strcmp(const char *str1, const char *str2);
char* strncpy(char *dest, const char *src, DWORD n);
int strncmp(const char *str1, const char *str2, DWORD n);
DWORD strlen(const char *str);
char* strcat(char* dest, const char* src);
char* strcpy(char* dest, const char* src);
char* strrchr(const char* str, int c);

#endif