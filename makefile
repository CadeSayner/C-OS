CC = gcc
CFLAGS = -m32 -fno-stack-protector -fno-builtin
LFlAGS = -m elf_i386 -T linker.ld -o kernel
AC = nasm
AFLAGS = -f elf32 

kernel: kernel.o vga.o boot gdt.o utils.o idt.o memory.o
	ld $(LFlAGS) obj/boot.o obj/kernel.o obj/vga.o obj/gdts.o obj/gdt.o obj/utils.o obj/idt.o obj/idts.o obj/memory.o

kernel.o: src/kernel.c 
	$(CC) $(CFLAGS) -c src/kernel.c -o obj/kernel.o

vga.o: src/vga.c
	$(CC) $(CFLAGS) -c src/vga.c -o obj/vga.o

gdt.o : src/gdt.c 
	$(CC) $(CFLAGS) -c src/gdt.c -o obj/gdt.o 

memory.o : src/memory.c 
	$(CC) $(CFLAGS) -c src/memory.c -o obj/memory.o 

utils.o : src/utils.c
	$(CC) $(CFLAGS) -c src/utils.c -o obj/utils.o 

idt.o: src/idt.c 
	$(CC) $(CFLAGS) -c src/idt.c -o obj/idt.o 

boot: src/boot.s
	$(AC) $(AFLAGS) src/boot.s -o obj/boot.o
	$(AC) $(AFLAGS) src/gdt.s -o obj/gdts.o
	$(AC) $(AFLAGS) src/idt.s -o obj/idts.o

.PHONY: run
run: kernel
	mv kernel COS/boot/kernel
	grub-mkrescue -o COS.iso COS/
	qemu-system-i386 COS.iso
	


