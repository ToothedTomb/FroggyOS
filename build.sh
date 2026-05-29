#!/bin/bash

# Assemble boot.s
as --32 boot.s -o boot.o

# Compile kernel.c (comment out the #error if it exists)
gcc -m32 -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Link the kernel
ld -m elf_i386 -T linker.ld -o kernel.bin boot.o kernel.o

# Check if multiboot compliant
if grub-file --is-x86-multiboot kernel.bin; then
    echo "✅ Kernel is Multiboot compliant"
else
    echo "❌ Kernel is NOT Multiboot compliant"
fi

# Create ISO directory structure
mkdir -p iso/boot/grub
cp kernel.bin iso/boot/

# Create GRUB config
cat > iso/boot/grub/grub.cfg << EOF
set timeout=0
set default=0

menuentry "FroggyOS" {
    multiboot /boot/kernel.bin
    boot
}
EOF

# Build ISO
grub-mkrescue -o froggyos.iso iso/

echo "✅ ISO created: froggyos.iso"
echo "Run with: qemu-system-x86_64 -cdrom froggyos.iso"
