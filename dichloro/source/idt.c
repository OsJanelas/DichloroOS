#include "variables.h"
#include "ports.h"
#include "text.h"
#include "memory.h"
#include "error.h"

#include "idt.h"

extern void load_idt(DWORD);

struct IDT_GateDescriptor idt[TOTALGATES];
struct IDT_Descriptor idtr;

void setupgate(BYTE entry, DWORD isr, WORD selector, BYTE flags)
{
    idt[entry].selector = selector;
    idt[entry].reserved = 0;
    idt[entry].flags = flags;
    idt[entry].offset = isr & 0xFFFF;
    idt[entry].offset2 = (isr >> 16) & 0xFFFF;
}

void setupidt(void)
{
    int IDTSize = sizeof(struct IDT_GateDescriptor) * TOTALGATES;

    idtr.size   = IDTSize - 1;
    idtr.offset = (DWORD) &idt;

    memoryset(&idt, 0, IDTSize);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    
    outb(0x21, 0x00);
    outb(0xA1, 0x00);

    setupgate(0, (DWORD)isr0,0x08, 0x8E);
    setupgate(1, (DWORD)isr1,0x08, 0x8E);
    setupgate(2, (DWORD)isr2,0x08, 0x8E);
    setupgate(3, (DWORD)isr3,0x08, 0x8E);
    setupgate(4, (DWORD)isr4, 0x08, 0x8E);
    setupgate(5, (DWORD)isr5, 0x08, 0x8E);
    setupgate(6, (DWORD)isr6, 0x08, 0x8E);
    setupgate(7, (DWORD)isr7, 0x08, 0x8E);
    setupgate(8, (DWORD)isr8, 0x08, 0x8E);
    setupgate(9, (DWORD)isr9, 0x08, 0x8E);
    setupgate(10, (DWORD)isr10, 0x08, 0x8E);
    setupgate(11, (DWORD)isr11, 0x08, 0x8E);
    setupgate(12, (DWORD)isr12, 0x08, 0x8E);
    setupgate(13, (DWORD)isr13, 0x08, 0x8E);
    setupgate(14, (DWORD)isr14, 0x08, 0x8E);
    setupgate(15, (DWORD)isr15, 0x08, 0x8E);
    setupgate(16, (DWORD)isr16, 0x08, 0x8E);
    setupgate(17, (DWORD)isr17, 0x08, 0x8E);
    setupgate(18, (DWORD)isr18, 0x08, 0x8E);
    setupgate(19, (DWORD)isr19, 0x08, 0x8E);
    setupgate(20, (DWORD)isr20, 0x08, 0x8E);
    setupgate(21, (DWORD)isr21, 0x08, 0x8E);
    setupgate(22, (DWORD)isr22, 0x08, 0x8E);
    setupgate(23, (DWORD)isr23, 0x08, 0x8E);
    setupgate(24, (DWORD)isr24, 0x08, 0x8E);
    setupgate(25, (DWORD)isr25, 0x08, 0x8E);
    setupgate(26, (DWORD)isr26, 0x08, 0x8E);
    setupgate(27, (DWORD)isr27, 0x08, 0x8E);
    setupgate(28, (DWORD)isr28, 0x08, 0x8E);
    setupgate(29, (DWORD)isr29, 0x08, 0x8E);
    setupgate(30, (DWORD)isr30, 0x08, 0x8E);
    setupgate(31, (DWORD)isr31, 0x08, 0x8E);

    //Interrupt Request Lines
    setupgate(32, (DWORD)irq0, 0x08, 0x8E);
    setupgate(33, (DWORD)irq1, 0x08, 0x8E);
    setupgate(34, (DWORD)irq2, 0x08, 0x8E);
    setupgate(35, (DWORD)irq3, 0x08, 0x8E);
    setupgate(36, (DWORD)irq4, 0x08, 0x8E);
    setupgate(37, (DWORD)irq5, 0x08, 0x8E);
    setupgate(38, (DWORD)irq6, 0x08, 0x8E);
    setupgate(39, (DWORD)irq7, 0x08, 0x8E);
    setupgate(40, (DWORD)irq8, 0x08, 0x8E);
    setupgate(41, (DWORD)irq9, 0x08, 0x8E);
    setupgate(42, (DWORD)irq10, 0x08, 0x8E);
    setupgate(43, (DWORD)irq11, 0x08, 0x8E);
    setupgate(44, (DWORD)irq12, 0x08, 0x8E);
    setupgate(45, (DWORD)irq13, 0x08, 0x8E);
    setupgate(46, (DWORD)irq14, 0x08, 0x8E);
    setupgate(47, (DWORD)irq15, 0x08, 0x8E);

    setupgate(128, (DWORD)isr128, 0x08, 0x8E);
    setupgate(177, (DWORD)isr177, 0x08, 0x8E);

    load_idt((DWORD)&idtr);
}

const char* exception_messages[] = 
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
};

void isrhandler(struct InterruptRegisters* regs)
{
    if (regs->int_no < 32)
    {
        errscreen(exception_messages[regs->int_no]);

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

        Print("\nESI: ", 0x0F);
        printhex(regs->esi, 0x0F);

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

void irqinstallhan(int irq, void (*handler)(struct InterruptRegisters *r))
{
    irqroutines[irq] = handler;
}

void irquninstallhan(int irq)
{
    irqroutines[irq] = 0;
}

void irqhandler(struct InterruptRegisters* regs)
{
    void (*handler)(struct InterruptRegisters *regs)
    
    = irqroutines[regs->int_no - 32];

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