#include "sys_exit.h"
#include "process.h"
#include "terminal.h"
#include "keyboard.h"

void sys_exit(struct InterruptRegisters *regs){
    enable_echoing(); 
    terminate_current_process();
    terminal_resume();
}