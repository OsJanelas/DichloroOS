#include "variables.h"
#include "ports.h"
#include "idt.h"
#include "allocate.h"
#include "memory.h"
#include "board.h"
#include "text.h"

#include "config.h"
#include "icmpconfig.h"
#include "utils.h"
#include "net.h"
#include "iptcp.h"


/*
    NETWORK UTILS
    (utils.h)
*/

WORD htons(WORD hostshort)
{
    return (hostshort << 8) | (hostshort >> 8);
}

DWORD hton(DWORD hostlong)
{
    return ((hostlong & 0x000000FF) << 24) |
           ((hostlong & 0x0000FF00) << 8)  |
           ((hostlong & 0x00FF0000) >> 8)  |
           ((hostlong & 0xFF000000) >> 24);
}

DWORD ntoh(DWORD netlong)
{
    return ((netlong & 0x000000FF) << 24) |
           ((netlong & 0x0000FF00) << 8)  |
           ((netlong & 0x00FF0000) >> 8)  |
           ((netlong & 0xFF000000) >> 24);
}

WORD ntohs(WORD netshort)
{
    return (netshort << 8) | (netshort >> 8);
}

//-------------------------------------------------------------

/*
    NETWORK SYSTEM
    (utils.h)
*/

WORD calculatechecksummary(LPDWORD addr, int lenght)
{
    DWORD summary = 0x00;

    while (lenght > 1)
    {
        summary += *addr++;
        lenght -= 2;
    }

    if (lenght == 1)
    {
        summary += *(LPBYTE) addr;
    }

    summary = (summary >> 16) + (summary & 0xFFFF);
    summary += (summary >> 16);

    return (WORD)(~summary);
}

DWORD iplabeltodword(const char* ip)
{
    DWORD result = 0;
    int part = 0;

    while (*ip) 
    {
        if (*ip >= '0' && *ip <= '9') 
        {
            part = part * 10 + (*ip - '0');
        } 
        else if (*ip == '.') 
        {
            result = (result << 8) | (part & 0xFF);
            
            part = 0;
        }

        ip++;
    }

    result = (result << 8) | (part & 0xFF);

    return result;
}

//-------------------------------------------------------------

/*
    NETWORK CONFIGURATION
    (net.h and config.h)
*/

rtl8139 rtl8139device;

static DWORD rxbuffer;
static DWORD iobase;

static BYTE currentTx = 0;

static int readptr;
static char transmitdesc;

static BYTE RTL8139SLOT;
static BYTE RTL8139BUS;

BYTE* getmac()
{
    rtl8139device.mac[0] = inb(iobase + 0x00);
    rtl8139device.mac[1] = inb(iobase + 0x01);
    rtl8139device.mac[2] = inb(iobase + 0x02);
    rtl8139device.mac[3] = inb(iobase + 0x03);
    rtl8139device.mac[4] = inb(iobase + 0x04);
    rtl8139device.mac[5] = inb(iobase + 0x05);

    return rtl8139device.mac;
}

DWORD rtl8139finddevice()
{
    for (BYTE bus = 0; bus < 256; bus++)
    {
        for (BYTE slot = 0; slot < 32; slot++)
        {
            DWORD vendorDevice = pciconfigreadword(bus, slot, 0, 0x00);

            if (vendorDevice == ((RTL8139_DEVICE_ID << 16) | RTL8139_VENDOR_ID))
            {
                DWORD bar = pciconfigreadword(bus, slot, 0, 0x10);

                if (bar & 0x01)
                {
                    RTL8139BUS  = bus;
                    RTL8139SLOT = slot;

                    return bar & ~0x3;
                }
            }
        }
    }

    return 0x00;
}

void startrtl8139()
{
    outb(iobase + 0x52, 0x00);

    outb(iobase + RTL8139_REG_COMMAND, RTL8139_CMD_RESET);

    while ((inb(iobase + RTL8139_REG_COMMAND) & RTL8139_CMD_RESET) != 0x00) 
    { 
    }

    getmac();
    
    rxbuffer = (DWORD)allocatememory(8192 + 16);

    if (!rxbuffer)
    {
        Debug("ERROR: Fail to allocate memory", 0x01);
        return;
    }

    outl(iobase + RTL8139_REG_RX_ADDR_LOW, rxbuffer);

    outw(iobase + RTL8139_REG_INTR_STATUS, 0xFFFF);
    outb(iobase + RTL8139_REG_COMMAND, 0x0C);

    outb(iobase + RTL8139_REG_COMMAND, RTL8139_CMD_RX_EN | RTL8139_CMD_TX_EN);
}

void RTL8139sendpacket(void* packet, WORD length, BYTE* dstMac, WORD etherType);
{
    EthernetFrame ethFrame;
    memorycopy(ethFrame.dstMac, dstMac, 6);
    memorycopy(ethFrame.srcMac, getmac(), 6);
    ethFrame.etherType = htons(etherType);

    BYTE txBuffer[sizeof(EthernetFrame) + length];
    memorycopy(txBuffer, &ethFrame, sizeof(EthernetFrame));
    memorycopy(txBuffer + sizeof(EthernetFrame), packet, length);

    DWORD txAddr = RTL8139_REG_TX_ADDR_LOW + (currentTx * 4);
    DWORD txStatus = RTL8139_REG_TX_ADDR_HIGH + (currentTx * 4);

    memorycopy((void*)inl(iobase + txAddr), txBuffer, sizeof(txBuffer));
    outl(iobase + txStatus, sizeof(txBuffer) & 0xFFFF);

    currentTx = (currentTx + 1) % 4;
}

int RTL8139getpacket(LPBYTE buffer, int bufferLength)
{
    WORD packetlenght = *(LPDWORD)(rxbuffer + readptr + 2);

    if (packetlenght == 0 || packetlenght > bufferLength)
    {
        return 0;
    }

    memorycopy(buffer, (void*)(rxbuffer + readptr), packetlenght);

    readptr = (readptr + packetlenght + 3) & (~3);

    outw(iobase + 0x38, readptr - 0x10);

    return packetlenght;
}

void rtl8139handler()
{
    WORD intrstatus = inw(iobase + RTL8139_REG_INTR_STATUS);
    
    if (intrstatus & 0x01)
    {
        Debug("Packet get!\n\n", 0x02);

        BYTE buff[2048];
        int lenght =  RTL8139getpacket(buff, sizeof(buff));

        if (lenght > 0)
        {
            EthernetFrame* ethfram = (EthernetFrame*)buff;
            IPHeader* ipH = (IPHeader*)(buff + sizeof(EthernetFrame));

            if (ntohs(ethfram->etherType) == ETHERNETIP)
            {
                if (ipH->protocol == 1)
                {
                    icmpheader* icmph = (icmpheader*)(buff + sizeof(EthernetFrame) + sizeof(IPHeader));
                    icmpreply(icmph, lenght - sizeof(EthernetFrame) - sizeof(IPHeader));
                }
            }
        }
    }

    outw(iobase + RTL8139_REG_INTR_STATUS, intrstatus);
}

void ethernetstart()
{
    iobase = rtl8139finddevice();

    if (iobase == 0x00)
    {
        Debug("RTL DEVICE NOT FOUND", 0x01);
    }
    else
    {
        startrtl8139();

        BYTE irq = pciconfigreadword(RTL8139BUS, RTL8139SLOT, 0x00, 0x3C) & 0xFF;
        irqinstallhan(irq, &rtl8139handler);
    }
}