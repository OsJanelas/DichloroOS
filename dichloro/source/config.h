// NETWORK CONFIG

#define RTL8139_REG_COMMAND      0x37
#define RTL8139_REG_TX_ADDR_LOW  0x20
#define RTL8139_REG_TX_ADDR_HIGH 0x24
#define RTL8139_REG_RX_ADDR_LOW  0x30
#define RTL8139_REG_RX_ADDR_HIGH 0x34
#define RTL8139_REG_INTR_MASK    0x3C
#define RTL8139_REG_INTR_STATUS  0x3E

#define RTL8139_CMD_RESET        0x10
#define RTL8139_CMD_RX_EN        0x08
#define RTL8139_CMD_TX_EN        0x04

#define RTL8139_VENDOR_ID        0x10EC
#define RTL8139_DEVICE_ID        0x8139

#define ICMP_ECHO_REQUEST        0x08
#define ICMP_ECHO_REPLY          0x00

#define RTL8139_IRQ              10

#define ROK (1<<0)
#define RER	(1<<1)
#define TOK (1<<2)
#define TER	(1<<3)

typedef struct
{
    DWORD txbuffer;
    DWORD rxbuffer;
    BYTE  mac[6];
} rtl8139;

DWORD rtl8139finddevice();
BYTE* recievemac();

void RTL8139sendpacket(void* packet, WORD length, BYTE* dstMac, WORD etherType);
int RTL8139getpacket(LPBYTE buffer, int bufferLength);

void startrtl8139();
void rtl8139handler();
void ethernetstart();