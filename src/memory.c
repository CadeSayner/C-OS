#include "stdint.h"
#include "multiboot.h"
#include "vga.h"
#include "memory.h"
#include "utils.h"

static uint32_t initialAllocationFrame;

void initMemory(uint32_t memLow){
    // invalidate the first PTE, since we no longer need it
   initial_page_dir[0] = 0;
   invalidate(0);
}

void invalidate(uint32_t vaddr){
    asm volatile ("invlpg %0" :: "m"(vaddr));
}



