#define ARP_REQUEST          0x01
#define ARP_REPLY            0x02
#define ARP_HW_TYPE_ETHERNET 0x01
#define ARP_PROTO_IP         0x0800
#define ARP_HW_ADDR_LEN      0x06
#define ARP_PROTO_ADDR_LEN   0x04
#define ARP_TABLE_SIZE       0x0A

typedef struct
{
    WORD  hwtype;
    WORD  prototype;
    BYTE  hwaddrlen;
    BYTE  protoaddrlen;
    WORD  opcode;
    BYTE  senderhwaddr[ARP_HW_ADDR_LEN];
    DWORD senderprotoaddr;
    BYTE  targethwaddr[ARP_HW_ADDR_LEN];
    DWORD targetprotoaddr;
} arpheader;

typedef struct
{
    DWORD ipAddress;
    BYTE  macAddress[ARP_HW_ADDR_LEN];
} arpentry;

void startarp();
void arprequest(DWORD targetIp);
void arpreply(arpheader *arpHdr);
BYTE arplookup(DWORD ip, LPBYTE mac);
void arpreceivepacket(LPBYTE packet, int length);