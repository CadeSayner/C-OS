#include "stdint.h"
#include "multiboot.h"
uint32_t get_process_count();
void terminal_resume();

void terminal_start(struct module processes[100]);