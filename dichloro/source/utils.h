// NETWORK UTILS

WORD htons(WORD hostshort);
WORD ntohs(WORD netshort);

DWORD hton(DWORD hostlong);
DWORD ntoh(DWORD netlong);

WORD calculatechecksummary(LPDWORD addr, int lenght);
DWORD iplabeltodword(const char* ip);