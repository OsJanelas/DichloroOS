#include "variables.h"
#include "allocate.h"
#include "winsys.h"
#include "arp.h"
#include "ata.h"
#include "board.h"
#include "boot.h"
#include "box.h"
#include "config.h"
#include "cursor.h"
#include "date.h"
#include "desktop.h"
#include "disk.h"
#include "elf.h"
#include "error.h"
#include "filesystem.h"
#include "gdt.h"
#include "gl.h"
#include "graphics.h"
#include "gui.h"
#include "icmpconfig.h"
#include "idt.h"
#include "kb.h"
#include "login.h"
#include "math.h"
#include "memfilesys.h"
#include "memory.h"
#include "mouse.h"
#include "net.h"
#include "notepad.h"
#include "ports.h"
#include "power.h"
#include "processor.h"
#include "shell.h"
#include "text.h"
#include "timer.h"
#include "userspace.h"
#include "utils.h"
#include "video.h"
#include "virtmem.h"

extern char scroll[];

void main(LPDWORD magic, struct multiboot_info* mbinfo, DWORD addr)
{
    // LOAD GRAPHICS AND VIDEO MODE
    initgraphics(magic);
    Debug("FRAMEBUFFER STARTED\n\n", 0x00);

    Debug("VESA Framebuffer: ", 0x00);
    printhex(getframebuff(), 0xFFFFFFFF);
    Print("\n", 0x00);

    Debug("VESA Pitch: ", 0x00);
    printhex(getpitch(), 0xFFFFFFFF);
    Print("\n", 0x00);

    if (addr != 0x2BADB002)
    {
        Debug("ERROR: Invalid Magic Number!\n", 0x01);
        return;
    }

    Debug("SUCCESS: Valid Magic Number!: 0x2BADB002\n", 0x00);

    for (int i = 0; i < mbinfo->mmap_length; i += sizeof(struct multiboot_mmap_entry))
    {
        struct multiboot_mmap_entry* entry = (struct multiboot_mmap_entry*)(mbinfo->mmap_addr + i);

        Debug("Low Addr: ", 2);
        printhex(entry->addr_low, 0xFFFFFFFF);
        Print(" High Addr: ", 0xFFFFFFFF);
        printhex(entry->addr_high, 0xFFFFFFF);
        Print(" Low length: ", 0xFFFFFFFF);
        printhex(entry->len_low, 0xFFFFFFFF);
        Print(" High length: ", 0xFFFFFFFF);
        printhex(entry->len_high, 0xFFFFFFFF);
        Print("  Size: ", 0xFFFFFFFF);
        printint(entry->size, 0xFFFFFFFF);
        Print(" Type: ", 0xFFFFFFFF);
        printint(entry->type, 0xFFFFFFFF);
        Print("\n", 0x00);
    }

    // LOAD KERNEL
    Debug("System loaded!\n", 0x00);
    
    // LOAD GLOBAL DESCRIPTOR TABLE
    initgdt();
    Debug("GDT and TSS started\n", 0x00);

    // LOAD INTERRUPTS
    setidt();
    Debug("IDT loaded\n", 0x00);

    // START VIRTUAL MEMORY MANAGER
    startvirtmem();
    Debug("Started Virtual Memory Manager\n", 0x00);

    // START FILE SYSTEM AND CREATE FILES AND FOLDERS
    initfs();
    createdir("bin");
    changedir("bin");
    createfile("scroll.exe", scroll, scrollsize, PERMX);
    changedir("..");
    createdir("other");
    createdir("home");
    createdir("develop");
    createdir("temporary");
    Debug("System files and folders created!\n", 0x00);

    // START TIMER
    inittimer();
    Debug("Loaded timer\n", 0x00);
    
    // START KEYBOARD DRIVER
    initkeyboard();
    Debug("Keyboard driver installed\n", 0x00);

    // START MOUSE DRIVER
    initmouse();
    SetCursorX(0x00);
    Debug("Mouse driver installed\n", 0x00);

    // START MEMORY
    initmemory();
    Debug("Memory started\n", 0x00);

    // START ETHERNET
    ethernetstart();
    startarp();
    Debug("Ethernet loaded\n", 0x00);

    // SETUP AUDIO BOARD (Sound Blaster 16)
    setupaudioboard();
    Debug("Audio driver installed", 0x00);

    // SHOW ALL ATAs
    identifyata(0, 0);
    identifyata(1, 0);
    Debug("Masters identifieds\n", 0x00);

    // INIT SHOW DATE SYSTEM
    showcmosmem();
    Debug("CMOS date system started\n", 0x00);

    // INIT HARDWARE SYSTEM
    alldisks();
    showcpuinfo();
    showdevices();
    Debug("Hardware started", 0x00);

    // WAIT
    sleep(4);

    // CLEAR SCREEN AND INIT BOOTSECTOR
    clearscren();
    bootsector();

    // START TERMINAL AND USERSPACE
    startshell();
    userspace();
}