CC = gcc
CFLAGS = -g -m32 -fno-stack-protector -fno-builtin -I ../libc/include

c_code:
	$(CC) $(CFLAGS) -c hello.c -o hello.o
	$(CC) $(CFLAGS) -c snake.c -o snake.o

elf_file:
	ld -m elf_i386 -o hello.elf hello.o -L../libc -l:libc.a -e main
	ld -m elf_i386 -o snake.elf snake.o -L../libc -l:libc.a -e main

