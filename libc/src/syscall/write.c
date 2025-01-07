
#include "syscalls.h"

void write(const void *buf, unsigned long count){
    syscall(1, 1, buf, count);
}

void update_echo(int enable){
    syscall(43, enable, 0, 0);
}