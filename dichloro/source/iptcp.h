#define MYIP       0xC0A80001
#define MYSUBNET   0xFFFFFF00
#define MYGATEWAY  0xC0A800FE

#define ETHERNETIP 0x0800
#define TCPPORT    80

typedef struct
{
    BYTE dstMac[6];
    BYTE srcMac[6];
    WORD etherType;
} EthernetFrame;

typedef struct
{
    BYTE  versionIhl;
    BYTE  dscpEcn;
    WORD  totalLength;
    WORD  id;
    WORD  flagsOffset;
    BYTE  ttl;
    BYTE  protocol;
    WORD  checksum;
    DWORD srcIp;
    DWORD dstIp;
} IPHeader;

typedef struct 
{
    WORD  srcPort;
    WORD  dstPort;
    DWORD seqNum;
    DWORD ackNum;
    BYTE  offsetReserved;
    BYTE  flags;
    WORD  window;
    WORD  checksum;
    WORD  urgentPtr;
} TCPHeader;

void sendpacket(LPBYTE data, WORD length, DWORD dstIp, WORD dstPort);
void getpacket();