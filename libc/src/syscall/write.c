
#include "syscalls.h"

void write(const void *buf, unsigned long count){
    syscall(1, 1, buf, count);
}