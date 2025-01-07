
#include "syscalls.h"

void write(const void *buf, unsigned long count){
    syscall(1, 1, buf, count);
}

void update_echo(int enable){
    syscall(43, enable, 0, 0);
}

void write_char_at(char chr, int column, int row){
    syscall(42, chr, column, row);
}

void reset_screen(){
    syscall(41, 0, 0, 0);
}