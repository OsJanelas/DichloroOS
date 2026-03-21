MBOOTPAGEALIGN EQU 1 << 0
MBOOTMEMINFO   EQU 1 << 1
MBOOTMAGIC     EQU 0x1BADB002
MBOOTFLAGS     EQU MBOOTPAGEALIGN MBOOTMEM_INFO
MBOOTCHECKSUM  EQU -(MBOOTMAGIC + MBOOTFLAGS)

section .multiboot

align 4
    dd MBOOTMAGIC
    dd MBOOTFLAGS
    dd MBOOTCHECKSUM
    dd 0, 0, 0, 0, 0

    dd 1
    dd 1280
    dd 720
    dd 32

section .bss

align 16

stacktop:

section .text

global start

; IMAGES
[GLOBAL bootsec]
[GLOBAL wallpap]

; GENERAL ICONS
[GLOBAL     error]
[GLOBAL   warning]
[GLOBAL  question]
[GLOBAL  asterisk]
[GLOBAL      user]
[GLOBAL     user2]
[GLOBAL     shell]

; FILES AND DIRECTORY ICONS
[GLOBAL       non]
[GLOBAL       txt]
[GLOBAL       dir]
[GLOBAL       exe]

; PROGRAMS
[GLOBAL  starsSize]
[GLOBAL      stars]

start:
    push ebx
    xor ebp, ebp

    extern main
    call main

haltkern:
    hlt

    jmp haltkern

section .data

; IMAGES (bmp)
%include "localdisk/dichloroos/system/include/bg.bmp"
%include "localdisk/dichloroos/system/include/dir.bmp"

; GENERAL ICONS (vad)

; FILES AND DIR (vad)

; PROGRAMS (elf)