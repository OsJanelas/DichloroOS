nasm -f bin localdisk/dichloroos/system/boot.asm -o localdisk/dichloroos/system/binaries/boot.bin
nasm -f bin localdisk/dichloroos/system/gdt/gdt.asm -o localdisk/dichloroos/system/binaries/gdt.o
nasm -f bin localdisk/dichloroos/system/interrupts/interrupts.asm -o localdisk/dichloroos/system/binaries/interrupts.o

gcc -m32 -c localdisk/dichloroos/system/include/ports.c -o localdisk/dichloroos/system/binaries/ports.o -ffreestanding
gcc -m32 -c localdisk/dichloroos/system/include/math.c -o localdisk/dichloroos/system/binaries/math.o -ffreestanding
gcc -m32 -c localdisk/dichloroos/system/graphics.c -o localdisk/dichloroos/system/binaries/graphics.o -ffreestanding
gcc -m32 -c localdisk/dichloroos/system/gl.c -o localdisk/dichloroos/system/binaries/gl.o -ffreestanding
