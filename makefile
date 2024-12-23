CC = gcc
CFLAGS = -m32 -fno-stack-protector -fno-builtin
LFlAGS = -m elf_i386 -T linker.ld -o kernel
AC = nasm
AFLAGS = -f elf32 

kernel: kernel.o vga.o boot.o
	ld $(LFlAGS) boot.o kernel.o vga.o

kernel.o: kernel.c 
	$(CC) $(CFLAGS) -c main.c -o kernel.o

vga.o: vga.c
	$(CC) $(CFLAGS) -c vga.c -o vga.o

boot.o: boot.s
	$(AC) $(AFLAGS) boot.s -o boot.o

.PHONY: run
run: kernel
	mv kernel COS/boot/kernel
	grub-mkrescue -o COS.iso COS/
	qemu-system-i386 COS.iso
	


