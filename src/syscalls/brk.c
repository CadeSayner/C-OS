#include "brk.h"
#include "process.h"
#include "utils.h"

uint32_t brk(struct InterruptRegisters *regs){
    // go through the process list and try and update it
    uint32_t vaddr = regs->ebx;
    if(vaddr == 0){
        int brk = get_current_process_brk();
        return brk;
    }else{
        int brk = set_current_process_brk(vaddr);
        return brk;
    }
}