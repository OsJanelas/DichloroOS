[bits 32]
[global loadidt]

loadidt:
    mov eax, [esp+4]
    lidt [eax]

    sti
    ret

%macro isnoerrorcode 1
    global isr%1

    isr%1:
        cli

        push long 0
        push long %1
        jmp isrcomonstub

%endmacro

%macro isnoerrorcode 1
    global isr%1

    isr%1:
        cli
        push long %1
        jmp isrcomonstub

%endmacro

%macro IRQ 2
    global irq%1

    irq%1:
        cli

        push long 0
        push long %2
        jmp irqcommonstub

%endmacro

;-------------------------------------------------------

isnoerrorcode 1
isnoerrorcode 2
isnoerrorcode 3
isnoerrorcode 4
isnoerrorcode 5
isnoerrorcode 6
isnoerrorcode 7
isnoerrorcode 8
isnoerrorcode 9
isnoerrorcode 10
isnoerrorcode 11
isnoerrorcode 12
isnoerrorcode 13
isnoerrorcode 14
isnoerrorcode 15
isnoerrorcode 16
isnoerrorcode 17
isnoerrorcode 18
isnoerrorcode 19
isnoerrorcode 20
isnoerrorcode 21
isnoerrorcode 22
isnoerrorcode 23
isnoerrorcode 24
isnoerrorcode 25
isnoerrorcode 26
isnoerrorcode 27
isnoerrorcode 28
isnoerrorcode 29
isnoerrorcode 30
isnoerrorcode 31
isnoerrorcode 128
isnoerrorcode 177

IRQ 0, 32
IRQ 1, 33
IRQ 2, 34
IRQ 3, 35
IRQ 4, 36
IRQ 5, 37
IRQ 6, 38
IRQ 7, 39
IRQ 8, 40
IRQ 9, 41
IRQ 10, 42
IRQ 11, 43
IRQ 12, 44
IRQ 13, 45
IRQ 14, 46
IRQ 15, 47

;-------------------------------------------------------

[EXTERN irshandler]

irscommonstub:
    pusha

    mov eax, ds

    push eax

    mov eax, cr2

    push eax

    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    
    call irshandler

    pop ebx

    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add esp, 0x08

    sti
    ret

[EXTERN irqhandler]

irqcommonstub:
    pusha

    mov eax, ds

    push eax

    mov eax, cr2

    push eax

    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    
    call irqhandler

    pop ebx

    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa

    add esp, 0x08

    sti
    ret