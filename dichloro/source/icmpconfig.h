#define ICMPPACKETSIZE 64

typedef struct
{
    BYTE type;
    BYTE code;
    WORD checksum;
    WORD id;
    WORD sequence;
} icmpheader;

void icmpreply(icmpheader* icmphdr, int lenght);
void sendping(const char* dstip);