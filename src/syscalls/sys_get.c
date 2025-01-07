#include "sys_write.h"
#include "vga.h"
#include "io.h"

char sys_get(struct InterruptRegisters *regs){
    return get_char();
}