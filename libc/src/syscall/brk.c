#include "syscalls.h"
#include "stdio.h"

int brk(void *addr){
    return syscall(45, addr, 0, 0);
}