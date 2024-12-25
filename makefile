CC = gcc
CFLAGS = -m32 -fno-stack-protector -fno-builtin
LFlAGS = -m elf_i386 -T linker.ld -o kernel
AC = nasm
AFLAGS = -f elf32 

kernel: kernel.o vga.o boot gdt.o
	ld $(LFlAGS) obj/boot.o obj/kernel.o obj/vga.o obj/gdts.o obj/gdt.o

kernel.o: src/kernel.c 
	$(CC) $(CFLAGS) -c src/kernel.c -o obj/kernel.o

vga.o: src/vga.c
	$(CC) $(CFLAGS) -c src/vga.c -o obj/vga.o

gdt.o : src/gdt.c 
	$(CC) $(CFLAGS) -c src/gdt.c -o obj/gdt.o 

boot: src/boot.s
	$(AC) $(AFLAGS) src/boot.s -o obj/boot.o
	$(AC) $(AFLAGS) src/gdt.s -o obj/gdts.o


.PHONY: run
run: kernel
	mv kernel COS/boot/kernel
	grub-mkrescue -o COS.iso COS/
	qemu-system-i386 COS.iso
	


