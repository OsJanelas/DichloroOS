#include "variables.h"
#include "memory.h"

#include "config.h"
#include "utils.h"
#include "arp.h"

arpentry arpTable[ARP_TABLE_SIZE];
int arpTableSize = 0x00;

void startarp()
{
    memoryset(arpTable, 0x00, sizeof(arpTable));
}

void arprequest(DWORD targetIp)
{
    arpheader arphdr;

    arphdr.hwtype = htons(ARP_HW_TYPE_ETHERNET);
    arphdr.prototype = htons(ARP_PROTO_IP);
    arphdr.hwaddrlen = ARP_HW_ADDR_LEN;
    arphdr.protoaddrlen = ARP_PROTO_ADDR_LEN;
    arphdr.opcode = htons(ARP_REQUEST);

    arphdr.senderprotoaddr = hton(0xC0A80001);
    arphdr.targetprotoaddr = hton(targetIp);

    memorycopy(arphdr.senderhwaddr, "\x00\x0C\x29\x3D\x59\x0A", ARP_HW_ADDR_LEN);
    memoryset(arphdr.targethwaddr, 0x00, ARP_HW_ADDR_LEN);

    RTL8139sendpacket(&arphdr, sizeof(arphdr), "\xFF\xFF\xFF\xFF\xFF\xFF", 0x01);
}

void arpreply(arpheader *arphdr)
{
    for (int i = 0; i < arpTableSize; i++) 
    {
        if (arpTable[i].ipAddress == ntoh(arphdr->senderprotoaddr)) 
        {
            return;
        }
    }

    if (arpTableSize < ARP_TABLE_SIZE)
    {
        arpentry* entry = &arpTable[arpTableSize++];

        entry->ipAddress = ntoh(arphdr->senderprotoaddr);
        memorycopy(entry->macAddress, arphdr->senderhwaddr, ARP_HW_ADDR_LEN);
    }
}

BYTE arplookup(DWORD ip, LPBYTE mac)
{
    for (int i = 0; i < arpTableSize; i++)
    {
        if (arpTable[i].ipAddress == ip)
        {
            memorycopy(mac, arpTable[i].macAddress, ARP_HW_ADDR_LEN);
            return TRUE;
        }
    }

    return FALSE;
}

void arprecievepacket(LPBYTE packet, int length)
{
    if (length < sizeof(arpheader))
    {
        return;
    }

    arpheader* arphdr = (arpheader*) packet;

    if (ntohs(arphdr->hwtype) != ARP_HW_TYPE_ETHERNET || ntohs(arphdr->prototype) != ARP_PROTO_IP)
    {
        return;
    }

    if (ntohs(arphdr->opcode) == ARP_REQUEST || ntohs(arphdr->opcode) == ARP_REPLY)
    {
        arpreply(arphdr);
    }
}