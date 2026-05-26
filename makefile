ASM = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -O2 -nostdlib -fno-pie -fno-stack-protector
LDFLAGS = -m elf_i386 -T linker.ld

all: froggy.bin

kernel_entry.o: kernel_entry.asm
	nasm -f elf32 kernel_entry.asm -o kernel_entry.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.bin: kernel_entry.o kernel.o
	$(LD) $(LDFLAGS) -o kernel.bin kernel_entry.o kernel.o

# boot.asm compiles to raw binary, not ELF
boot.bin: boot.asm
	nasm -f bin boot.asm -o boot.bin

# Combine bootloader and kernel
froggy.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > froggy.bin

run: froggy.bin
	qemu-system-i386 froggy.bin

clean:
	rm -f *.o *.bin
