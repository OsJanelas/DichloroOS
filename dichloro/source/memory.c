#include "variables.h"
#include "memory.h"

void* memoryset(void* dst, char val, DWORD num)
{
    LPBYTE tmp = (LPBYTE) dst;

    for (DWORD i = 0; i < num; i++)
    {
        tmp[i] = (BYTE) val;
    }

    return dst;
}

void* memorycopy(void *dest, const void *src, DWORD n)
{
    unsigned char *d = dest;
    const unsigned char *s = src;

    while (n--)
    {
        *d = *s;
        d++;
        s++;
    }

    return dest;
}

void* memorymove(void* dest, const void* src, DWORD n) 
{
    LPBYTE d = (LPBYTE) dest;
    const LPBYTE s = (LPBYTE) src;

    if (d < s) 
    {
        for (DWORD i = 0; i < n; i++)
         {
            d[i] = s[i];
        }
    } 
    else 
    {
        for (DWORD i = n; i > 0; i--) 
        {
            d[i - 1] = s[i - 1];
        }
    }

    return dest;
}

int memorycmp(const void *s1, const void *s2, DWORD n) 
{
    const LPBYTE p1 = (const LPBYTE)s1;
    const LPBYTE p2 = (const LPBYTE)s2;

    for (DWORD i = 0; i < n; i++) 
    {
        if (p1[i] != p2[i]) 
        {
            return (p1[i] < p2[i]) ? -1 : 1;
        }
    }

    return 0;
}

int strcmp(const char *str1, const char *str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }

    return *(unsigned char *)str1    -   *(unsigned char *) str2;
}

char* strncpy(char *dest, const char *src, DWORD n)
{
    DWORD i = 0;
    
    while (i < n && src[i] != '\0') 
    {
        dest[i] = src[i];
        i++;
    }
    
    while (i < n) 
    {
        dest[i] = '\0';
        i++;
    }
    
    return dest;
}

int strncmp(const char *str1, const char *str2, DWORD n)
{
    DWORD i = 0;

    while (i < n) 
    {
        if (str1[i] != str2[i]) 
        {
            return (unsigned char)str1[i] - (unsigned char)str2[i];
        }

        if (str1[i] == '\0' || str2[i] == '\0') 
        {
            break;
        }

        i++;
    }

    return 0;
}

DWORD strlen(const char *str)
{
    DWORD lenght = 0;
    
    while (str[lenght] != '\0')
    {
        lenght++;
    }

    return lenght;
}

char* strcat(char* dest, const char* src)
{
    char* ptr = dest;

    while (*ptr != '\0')
    {
        ptr++;
    }

    while (*src != '\0')
    {
        *ptr = *src;
        ptr++;
        src++;
    }

    *ptr = '\0';

    return dest;
}

char* strcpy(char* dest, const char* src)
{
    char* originaldest = dest;

    while (*src != '\0') 
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';

    return originaldest;
}

char* strrchr(const char* str, int c)
{
    char* last = NULL;

    while (*str)
    {
        if (*str == (char)c)
        {
            last = (char*)str;
        }
        
        str++;
    }

    return last;
}