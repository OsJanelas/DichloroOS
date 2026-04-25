# COMPILATE C FILES

gcc -ffreestanding -m32 -g -c dichloro/source/kernel.c        -o dichloro/binaries/system/kernel.o
gcc -ffreestanding -m32 -g -c dichloro/source/power.c        -o dichloro/binaries/drivers/power.o
gcc -ffreestanding -m32 -g -c dichloro/source/ports.c        -o dichloro/binaries/include/ports.o
gcc -ffreestanding -m32 -g -c dichloro/source/math.c        -o dichloro/binaries/include/math.o
gcc -ffreestanding -m32 -g -c dichloro/source/gl.c        -o dichloro/binaries/graphics/gl.o
gcc -ffreestanding -m32 -g -c dichloro/source/print.c        -o dichloro/binaries/graphics/print.o
gcc -ffreestanding -m32 -g -c dichloro/source/arp.c        -o dichloro/binaries/hardware/network/arp.o
gcc -ffreestanding -m32 -g -c dichloro/source/error.c        -o dichloro/binaries/system/error.o
gcc -ffreestanding -m32 -g -c dichloro/source/mouse.c        -o dichloro/binaries/drivers/mouse.o
gcc -ffreestanding -m32 -g -c dichloro/source/video.c        -o dichloro/binaries/graphics/video.o
gcc -ffreestanding -m32 -g -c dichloro/source/board.c        -o dichloro/binaries/hardware/board.o
gcc -ffreestanding -m32 -g -c dichloro/source/gdt.c        -o dichloro/binaries/gdt/gdt.o
gcc -ffreestanding -m32 -g -c dichloro/source/processor.c        -o dichloro/binaries/hardware/processor.o
gcc -ffreestanding -m32 -g -c dichloro/source/gdt.c        -o dichloro/binaries/gdt/gdt.o
gcc -ffreestanding -m32 -g -c dichloro/source/winsys.c        -o dichloro/binaries/system/winsys.o
gcc -ffreestanding -m32 -g -c dichloro/source/chemical_ui.c        -o dichloro/binaries/system/chemical_ui.o
gcc -ffreestanding -m32 -g -c dichloro/source/disk.c        -o dichloro/binaries/hardware/disk.o
gcc -ffreestanding -m32 -g -c dichloro/source/kb.c       -o dichloro/binaries/drivers/kb.o
gcc -ffreestanding -m32 -g -c dichloro/source/net.c       -o dichloro/binaries/hardware/network/net.o
gcc -ffreestanding -m32 -g -c dichloro/source/terminal.c       -o dichloro/binaries/system/terminal.o
gcc -ffreestanding -m32 -g -c dichloro/source/filesystem.c       -o dichloro/binaries/system/filesystem.o
gcc -ffreestanding -m32 -g -c dichloro/source/elf.c       -o dichloro/binaries/elf/elf.o
gcc -ffreestanding -m32 -g -c dichloro/source/memory.c       -o dichloro/binaries/hardware/memory/memory.o
gcc -ffreestanding -m32 -g -c dichloro/source/drive.c       -o dichloro/binaries/drivers/drive.o

# COMPILATE ASSEMBLY FILES

nasm -f bin dichloro/source/gdtable.asm         -o dichloro/binaries/gdt/gdtable.o
nasm -f bin dichloro/source/interrupts.asm         -o dichloro/binaries/system/interrupts.o

# COMPILATE PROGRAMS

gcc -ffreestanding -m32 -g -c dichloro/source/scroll.c        -o dichloro/binaries/system/programs/scroll.o