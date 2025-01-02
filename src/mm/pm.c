#include "memory.h"
#include "stdint.h"
#include "vga.h"
#include "utils.h"

static uint32_t proc_vaddr_start = 0x08048000;
static proc_stack_start = 0xB0000000;
static proc_stack_start_page = 0xB0000000/4096;

// get a page table mapping the process with entry point and starting point into 
uint32_t* get_page_dir(uint32_t proc_start, uint32_t proc_end){
    uint32_t* page_dir = allocPage();
    memset(page_dir, 0, 4096);
    uint32_t start_frame = proc_start >> 12;
    uint32_t start_page = proc_vaddr_start >> 12;
    uint32_t end_frame = proc_end >> 12;
    uint32_t num_phys_frames = end_frame - start_frame;

    for(uint16_t i=0; i < num_phys_frames; i++){
        add_mapping(page_dir, start_page+i, start_frame + i);
    }
    // map the stack in as well
    uint32_t stack_start = alloc_n_consecutive_pages(5)-0xC0000000;

    uint32_t stack_start_frame = stack_start/4096;


    for (int i = 0; i < 5; i++)
    {
        add_mapping(page_dir, proc_stack_start_page+i, stack_start_frame+i);
    }

    map_kernel(page_dir);
    return page_dir;
}

void map_kernel(uint32_t* page_dir){
    for (uint16_t i = 0; i < 255; i++)
    {
       page_dir[768 + i]  = (i << 22) | 0b10000011; 
    }
}

void dump_page_dir(uint32_t* page_dir){
    print("\n");
    for(uint16_t i = 0; i < 1024; i++){
        printHexInt(page_dir[i]);
        print(", ");
    }
}


// add a mapping in the provided page directory that maps from page_num to frame_num
void add_mapping(uint32_t* page_dir, uint32_t page_num, uint32_t frame_num){
    uint16_t page_dir_index = page_num >> 10;
    if(page_dir[page_dir_index] == 0){
        // then no page table entry exists for the page so add a 4kb aligned pagetable
        uint32_t* page_table = allocPage();
        page_dir[page_dir_index] = ((uint32_t)page_table - 0xC0000000) | 0b111;
    }
    // at this point we can rest assured that a page table exists
    uint16_t page_table_index = page_num % 1024;
    ((uint32_t*)((page_dir[page_dir_index] + 0xC0000000)&(~7)))[page_table_index] = (frame_num << 12) | 0b111;
}
