CC = gcc
CFLAGS = -m32 -nostdlib -fno-builtin -ffreestanding

all: libc.a

libc.a: c_src 
	ar rcs libc.a obj/sys_read.o obj/sys_write.o obj/brk.o

c_src: 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/brk.c -o obj/brk.o 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/sys_read.c -o obj/sys_read.o 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/sys_write.c -o obj/sys_write.o 

clean:
	rm -f src/**/*.o libc.a
