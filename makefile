CC = gcc
CFLAGS = -m32 -fno-stack-protector -fno-builtin -I include
LFlAGS = -m elf_i386 -T linker.ld -o kernel
AC = nasm
AFLAGS = -f elf32 

c_code:
	$(CC) $(CFLAGS) -c src/kernel.c -o obj/kernel.o
	$(CC) $(CFLAGS) -c src/drivers/vga.c -o obj/vga.o
	$(CC) $(CFLAGS) -c src/arch/gdt.c -o obj/gdt.o 
	$(CC) $(CFLAGS) -c src/mm/memory.c -o obj/memory.o 
	$(CC) $(CFLAGS) -c src/lib/utils.c -o obj/utils.o 
	$(CC) $(CFLAGS) -c src/arch/idt.c -o obj/idt.o
	$(CC) $(CFLAGS) -c src/lib/io.c -o obj/io.o
	$(CC) $(CFLAGS) -c src/drivers/keyboard.c -o obj/keyboard.o 

asm_code:
	$(AC) $(AFLAGS) src/arch/boot.s -o obj/boot.o
	$(AC) $(AFLAGS) src/arch/gdt.s -o obj/gdts.o
	$(AC) $(AFLAGS) src/arch/idt.s -o obj/idts.o

kernel: asm_code c_code
	ld $(LFlAGS) obj/boot.o obj/kernel.o obj/vga.o obj/gdts.o obj/gdt.o obj/utils.o obj/idt.o obj/idts.o obj/memory.o obj/keyboard.o obj/io.o

.PHONY: run
run: kernel
	mv kernel COS/boot/kernel
	grub-mkrescue -o COS.iso COS/
	qemu-system-i386 COS.iso
	


