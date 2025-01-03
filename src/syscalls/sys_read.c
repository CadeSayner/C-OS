
#include "sys_write.h"
#include "vga.h"
#include "io.h"

uint32_t read(struct InterruptRegisters *regs){
    const char* buf = regs->ebx;
    const uint32_t len = regs->ecx;
    kread(buf, len);
    return;
}