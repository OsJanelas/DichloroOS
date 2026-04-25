#define PCICONFIGADDRESS 0xCF8
#define PCICONFIGDATA 0xCFC

DWORD pciconfigreadword(BYTE bus, BYTE slot, BYTE func, BYTE offset);
void showdevices();