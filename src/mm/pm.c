#include "memory.h"
#include "stdint.h"
#include "vga.h"
#include "utils.h"
#include "pm.h"
#include "process.h"

static uint32_t proc_vaddr_start_ = 0x08048000;
static proc_stack_start = 0xB0000000;
static proc_stack_start_page = 0xB0000000/4096;

uint32_t get_physical_address_(uint32_t* page_dir, uint32_t vaddr){
    uint32_t page_dir_index = vaddr >> 22;
    uint32_t page_table_index = (vaddr >> 12) % 1024;
    //printHexInt(((uint32_t*)((page_dir[page_dir_index] & (~7)) + 0xC0000000)));
    return ((uint32_t*)((page_dir[page_dir_index] & (~7)) + 0xC0000000))[page_table_index];
}
// get a page table mapping the process with entry point and starting point into 
uint32_t* get_page_dir(uint32_t proc_start, uint32_t proc_end, uint32_t data_start, uint32_t data_offset){
    uint32_t* page_dir = allocPage();
    memset(page_dir, 0, 4096);
    uint32_t start_frame = proc_start >> 12;
    uint32_t start_page = proc_vaddr_start_ >> 12;
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

    // TODO: This assumes the data segment is < 4KB obviously not true in general
    add_mapping(page_dir, data_start >> 12, start_frame + (data_offset >> 12));

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


void page_fault_handler(uint32_t fault_vaddr){
    if(fault_vaddr <= get_current_process_brk() && fault_vaddr >= get_current_process_sbrk()){
        // print("we have a page fault in the heap baby");
        uint32_t page_addr = alloc_n_consecutive_pages(1);
        uint32_t phys_addr_npage = (page_addr - 0xC0000000) >> 12;
        add_mapping(get_current_process_page_dir(), fault_vaddr >> 12, phys_addr_npage);
        invalidate(fault_vaddr & 0xFFFFF000 );
        return;
    }
}

// add a mapping in the provided page directory that maps from page_num to frame_num
void add_mapping(uint32_t* page_dir, uint32_t page_num, uint32_t frame_num){
    uint16_t page_dir_index = page_num >> 10;
    if(page_dir[page_dir_index] == 0){
        // then no page table entry exists for the page so add a 4kb aligned pagetable
        uint32_t* page_table = allocPage();
        page_dir[page_dir_index] = (((uint32_t)page_table - 0xC0000000) | 0b111);
    }
    // at this point we can rest assured that a page table exists
    uint16_t page_table_index = page_num % 1024;
    
    ((uint32_t*)((page_dir[page_dir_index] + 0xC0000000)&(0xFFFFF000)))[page_table_index] = (frame_num << 12) | 0b111;
}
