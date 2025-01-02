#include "syscalls.h"

void read(const void *buf, size_t count){
    syscall(0, buf, count, 0);
    return;
}