[bits 16]
[ORG 0x7C00]

%define KERNEL 0x7E00

bootmain:
    cli
    cld

    xor ax, ax
    mov ds, ax
    mov es, ax

    mov sp, 0x7C00
    mov ss, ax

    sti

    mov ax, 0x4F02
    mov bx, 0x4103
    int 0x10

    call readsectors
    call setgdt

    jmp  CODESEG:pmode

readsectors:
    mov cx, 10

.loop:
    mov ah, 0x42
    mov si, DAP
    int 0x13

    add dword [DAP+8], 127
    add word [DAP+6], 0xFE0

    pop cx

    loop .loop

    ret

align 4

DAP:
    db 0x10
    db 0x00
    dw 0x7F
    dw 0x0000
    dw KERNEL >> 4
    dq 1

;-----------------------------------

%include "localdisk/dichloroos/system/gdt.asm"
%include "localdisk/dichloroos/system/pm.asm"