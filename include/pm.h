#include "stdint.h"

uint32_t* get_page_dir(uint32_t proc_start, uint32_t proc_end);

void page_fault_handler(uint32_t fault_vaddr);