#include "memory.h"
#include "stdint.h"
#include "vga.h"
#include "utils.h"

static uint32_t proc_vaddr_start = 0x08048000;

// get a page table mapping the process with entry point and starting point into 
uint32_t* get_page_dir(uint32_t proc_start, uint32_t proc_end){
    uint32_t* page_dir = kmalloc(1024);
    memset(page_dir, 0, 1024);
    uint32_t start_frame = proc_start >> 12;
    uint32_t start_page = proc_vaddr_start >> 12;
    uint32_t end_frame = proc_end >> 12;
    uint32_t num_phys_frames = end_frame - start_frame;

    for(uint16_t i=0; i < num_phys_frames; i++){
        // print("need to map\n");
        // printHexInt(start_frame + i);
        // print("\nto page\n");
        // printHexInt(start_page + i);
        // print("\n");
        add_mapping(page_dir, start_page+i, start_frame + i);
    }

    // uint32_t test_address = proc_vaddr_start + 4096;
    // uint32_t page_dir_index = (test_address >> 22);
    // uint32_t table_address = page_dir[page_dir_index];
    // uint32_t page_table_index = ((test_address) >> 12)%1024;
    // uint32_t final_entry = ((uint32_t*)(table_address))[page_table_index];
    // printHexInt(final_entry);

    // at this point the process itself is mapped in, now lets map in the kernel
    map_kernel(page_dir);
    return page_dir;
}

void map_kernel(uint32_t* page_dir){
    for (uint16_t i = 0; i < 255; i++)
    {
       page_dir[768 + i]  = (i << 22) | 0b10000011; 
    }
}

// add a mapping in the provided page directory that maps from page_num to frame_num
void add_mapping(uint32_t* page_dir, uint32_t page_num, uint32_t frame_num){
    uint16_t page_dir_index = page_num >> 10;
    if(page_dir[page_dir_index] == 0){
        // then no page table entry exists for the page so add a 4kb aligned pagetable
        uint32_t* page_table = allocPage();
        page_dir[page_dir_index] = (uint32_t)page_table;
    }
    // at this point we can rest assured that a page table exists
    uint16_t page_table_index = page_num % 1024;
    ((uint32_t*)page_dir[page_dir_index])[page_table_index] = (frame_num << 12) | 0b111;
}
