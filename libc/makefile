CC = gcc
CFLAGS = -g -O0 -m32 -fno-stack-protector -fno-builtin

all: libc.a

libc.a: c_src 
	ar rcs libc.a obj/read.o obj/write.o obj/brk.o obj/malloc.o obj/printf.o obj/syscall.o obj/print.o obj/get.o obj/exit.o

c_src: 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/brk.c -o obj/brk.o 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/read.c -o obj/read.o 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/write.c -o obj/write.o
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/get.c -o obj/get.o
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/exit.c -o obj/exit.o
	$(CC) $(CFLAGS) -Iinclude -c src/io/printf.c -o obj/printf.o 
	$(CC) $(CFLAGS) -Iinclude -c src/io/print.c -o obj/print.o 
	$(CC) $(CFLAGS) -Iinclude -c src/memory/malloc.c -o obj/malloc.o 
	$(CC) $(CFLAGS) -Iinclude -c src/syscall/syscall.c -o obj/syscall.o 

clean:
	rm -f src/**/*.o libc.a
