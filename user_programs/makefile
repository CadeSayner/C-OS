CC = gcc
CFLAGS = -g -m32 -fno-stack-protector -fno-builtin -I ../libc/include

c_code:
	$(CC) $(CFLAGS) -c hello.c -o hello.o
	$(CC) $(CFLAGS) -c pi_calculator.c -o pi_calculator.o

elf_file:
	ld -m elf_i386 -o module2.elf module_2.o -L../libc -l:libc.a -e main2

