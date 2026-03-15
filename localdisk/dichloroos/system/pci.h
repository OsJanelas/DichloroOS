#define PCICONFIGADRESS 0xCF8
#define PICCONFIGDATA 0xCFC

DWORD pciconfigreadword(BYTE bus, BYTE slot, BYTE func, BYTE offset)
void showpcidevices();