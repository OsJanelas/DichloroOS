; GLOBAL DESCRIPTOR TABLE

[bits 16]

setgdt:
    cli

    lgdt [gdtr]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ret

gdt:
    gdt.null:
        dd 0x00
        dd 0x00

    gdt.code:
        dw 0xFFFF
        dw 0x00
        db 0x00
        db 0x9A
        db 0xCF
        db 0x00

    gdt.data:
        dw 0xFFFF
        dw 0x00
        db 0x00
        db 0x92
        db 0xCF
        db 0x00

    gdtr:
        dw gdtr - gdt - 1
        dd gdt
    
    CODESEG equ gdt.code - gdt
    DATASEG equ gdt.data - gdt