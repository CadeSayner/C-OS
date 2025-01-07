#include "sys_write.h"
#include "vga.h";
#include "keyboard.h"

uint32_t write(struct InterruptRegisters *regs){
    const uint16_t file_descriptor  = regs->ebx;
    const void *buf = regs->ecx;
    const uint32_t len = regs->edx;
    if(file_descriptor != 1){
        print("ERROR: User process attempting to write into invalid file descriptor.");
        return;
    }
    print_len((char*)buf, len);
}

void update_echo(struct InterruptRegisters *regs){
    const uint8_t enable = regs->ebx;
    if(enable){
        enable_echoing();
    }else{
        disable_echoing();
    }
}
