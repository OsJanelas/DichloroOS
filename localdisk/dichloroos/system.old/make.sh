nasm -f bin localdisk/dichloroos/system/boot.asm -o localdisk/dichloroos/system/binaries/boot.bin
nasm -f bin localdisk/dichloroos/system/gdt/gdt.asm -o localdisk/dichloroos/system/binaries/gdt.o
nasm -f bin localdisk/dichloroos/system/interrupts/interrupts.asm -o localdisk/dichloroos/system/binaries/interrupts.o
