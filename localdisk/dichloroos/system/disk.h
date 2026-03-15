#define IDEPRIMARYCOMMANDPORT   0x1F0
#define IDEPRIMARYCONTROLPORT   0x3F6
#define IDESTATUSREADY           0x40
#define IDESTATUSERROR           0x01

#define FLOPPYSTATUSPORT         0x3F0
#define FLOPPYREADY               0x80

void waitforready(WORD base);
DWORD getdiskcapacity(WORD base);
DWORD getfloppycapacity(void);
void listdisks(void);