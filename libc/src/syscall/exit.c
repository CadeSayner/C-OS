#include "syscalls.h"

void exit(){
    syscall(40, 0, 0, 0);
}