[global gdtflush]

gdtflush:
    mov eax, [esp + 4]
    lgdt [eax]

    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:.flush

.flush:
    ret

[global tssflush]

tssflush:
    mov ax, 0x28
    ltr ax
    ret