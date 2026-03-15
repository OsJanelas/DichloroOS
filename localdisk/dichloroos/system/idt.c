#include "include/variables.h"
#include "include/ports.h"
#include "text.h"
#include "memory.h"
#include "err.h"

#include "idt.h"

extern void loadidt(DWORD);

struct IDTgatedescriptor idt[TOTALGATES];
struct IDTdescriptor idtr;

void setidtgate(BYTE entry, DWORD isr, WORD selector, BYTE flags)
{
    idt[entry].selector = selector;
    idt[entry].reserved = 0x00;
    idt[entry].flags = flags;

    idt[entry].offset  = isr & 0xFFFF;
    idt[entry].offset2 = (isr >> 16) & 0xFFFF;
}

void setidt(void)
{
    int IDTsize = (struct IDTgatedescriptor) * TOTALGATES;

    idtr.size = IDTsize - 1;
    idtr.offset = (DWORD) &idt

    memset(&idt, 0, IDTsize);

    // PIC INIT
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // INTERRUPTIONS BASE VECTOR
    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    // LINK PIC WITH OTHER
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // SETUP OPERATING MODE
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // SET PICs MASKS
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    setupidtgate(0, (DWORD)isr0, 0x08, 0x8E);
    setupidtgate(1, (DWORD)isr1, 0x08, 0x8E);
    setupidtgate(2, (DWORD)isr2, 0x08, 0x8E);
    setupidtgate(3, (DWORD)isr3, 0x08, 0x8E);
    setupidtgate(4, (DWORD)isr4, 0x08, 0x8E);
    setupidtgate(5, (DWORD)isr5, 0x08, 0x8E);
    setupidtgate(6, (DWORD)isr6, 0x08, 0x8E);
    setupidtgate(7, (DWORD)isr7, 0x08, 0x8E);
    setupidtgate(8, (DWORD)isr8, 0x08, 0x8E);
    setupidtgate(9, (DWORD)isr9, 0x08, 0x8E);
    setupidtgate(10, (DWORD)isr10, 0x08, 0x8E);
    setupidtgate(11, (DWORD)isr11, 0x08, 0x8E);
    setupidtgate(12, (DWORD)isr12, 0x08, 0x8E);
    setupidtgate(13, (DWORD)isr13, 0x08, 0x8E);
    setupidtgate(14, (DWORD)isr14, 0x08, 0x8E);
    setupidtgate(15, (DWORD)isr15, 0x08, 0x8E);
    setupidtgate(16, (DWORD)isr16, 0x08, 0x8E);
    setupidtgate(17, (DWORD)isr17, 0x08, 0x8E);
    setupidtgate(18, (DWORD)isr18, 0x08, 0x8E);
    setupidtgate(19, (DWORD)isr19, 0x08, 0x8E);
    setupidtgate(20, (DWORD)isr20, 0x08, 0x8E);
    setupidtgate(21, (DWORD)isr21, 0x08, 0x8E);
    setupidtgate(22, (DWORD)isr22, 0x08, 0x8E);
    setupidtgate(23, (DWORD)isr23, 0x08, 0x8E);
    setupidtgate(24, (DWORD)isr24, 0x08, 0x8E);
    setupidtgate(25, (DWORD)isr25, 0x08, 0x8E);
    setupidtgate(26, (DWORD)isr26, 0x08, 0x8E);
    setupidtgate(27, (DWORD)isr27, 0x08, 0x8E);
    setupidtgate(28, (DWORD)isr28, 0x08, 0x8E);
    setupidtgate(29, (DWORD)isr29, 0x08, 0x8E);
    setupidtgate(30, (DWORD)isr30, 0x08, 0x8E);
    setupidtgate(31, (DWORD)isr31, 0x08, 0x8E);

    setupidtgate(0, (DWORD)irq0, 0x08, 0x8E);
    setupidtgate(1, (DWORD)irq1, 0x08, 0x8E);
    setupidtgate(2, (DWORD)irq2, 0x08, 0x8E);
    setupidtgate(3, (DWORD)irq3, 0x08, 0x8E);
    setupidtgate(4, (DWORD)irq4, 0x08, 0x8E);
    setupidtgate(5, (DWORD)irq5, 0x08, 0x8E);
    setupidtgate(6, (DWORD)irq6, 0x08, 0x8E);
    setupidtgate(7, (DWORD)irq7, 0x08, 0x8E);
    setupidtgate(8, (DWORD)irq8, 0x08, 0x8E);
    setupidtgate(9, (DWORD)irq9, 0x08, 0x8E);
    setupidtgate(10, (DWORD)irq10, 0x08, 0x8E);
    setupidtgate(11, (DWORD)irq11, 0x08, 0x8E);
    setupidtgate(12, (DWORD)irq12, 0x08, 0x8E);
    setupidtgate(13, (DWORD)irq13, 0x08, 0x8E);
    setupidtgate(14, (DWORD)irq14, 0x08, 0x8E);
    setupidtgate(15, (DWORD)irq15, 0x08, 0x8E);

    setupidtgate(128, (DWORD)isr128, 0x08, 0x8E);
    setupidtgate(177, (DWORD)isr177, 0x08, 0x8E);
}

const char* exceptionmsgs[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Fault",
    "Machine Check", 
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
}

void isrhandler(struct interruptregisters* regs)
{
    if (regs->int_no < 32)
    {
        errscreen(exceptionmsgs[regs->int_no]);

        Print("\nEAX: ", 0x0F);
        printhex(regs->eax, 0x0F);

        Print("\nEBX: ", 0x0F);
        printhex(regs->ebx, 0x0F);

        Print("\nECX: ", 0x0F);
        printhex(regs->ecx, 0x0F);

        Print("\nEDX: ", 0x0F);
        printhex(regs->edx, 0x0F);

        Print("\nESP: ", 0x0F);
        printhex(regs->esp, 0x0F);

        Print("\nEBP: ", 0x0F);
        printhex(regs->ebp, 0x0F);

        Print("\nEDI: ", 0x0F);
        printhex(regs->edi, 0x0F);

        for (;;);
    }
}

void* irqroutines[16] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void IRQinstallhandler(int irq, void (*handler)(struct interruptregisters *r))
{
    irqroutines[irq] = handler;
}

void IRQuninstallhandler(int irq)
{
    irqroutines[irq] = 0;
}

void irqhandler(struct interruptregisters* regs)
{
    void (*handler)(struct interruptregisters *regs);

    handler = irqroutines[regs->int_no - 32];

    if (handler)
    {
        handler(regs);
    }

    if (regs->int_no >= 40)
    {
        outb(0xA0, 0x20);
    }

    outb(0x20, 0x20);
}
