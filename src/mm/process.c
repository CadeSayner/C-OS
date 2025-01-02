#include "process.h"
#include "pm.h"
#include "memory.h"
#include "utils.h"

static struct process* proc_list = 0; 

static int no_of_processes = 0;
extern void switch_to_user_mode_process(uint32_t e_entry);

// creates a process and returns the process id
uint16_t create_proc(uint32_t entry, uint32_t size, uint32_t proc_start, uint32_t proc_end){
    uint32_t* page_dir =  get_page_dir(proc_start, proc_end);
    struct process* proc = kmalloc(sizeof(struct process));
    // initialise the new process struct
    proc->entry_point = entry;
    proc->id = no_of_processes++;
    proc->page_dir = page_dir;
    proc->running = false;
    proc->next = 0;

    if(proc_list == 0){
        proc_list = proc;
    }else{
        // move to end of the linked list and append the process to the end
        struct process* iter = proc_list;
        while(iter->next != 0){
            iter = iter->next;     
        }
        iter->next = proc;
    }
    return proc->id;
}


struct process get_process(uint16_t id){
    struct process *curr = proc_list;
    for(uint16_t i = 0; i < id; i++){
        curr = curr->next;
    }
    return *curr;
}

bool check_page_dirs_equal(uint32_t* pd1, uint32_t* pd2){
    for(uint16_t i =0; i < 1024; i++){
        if(pd1[i] != 0 && pd2[i] !=0){
            uint32_t *pt1 = (uint32_t *)(pd1[i] + 0xC0000000);
            uint32_t *pt2 = (uint32_t *)(pd2[i] + 0xC0000000);
            for(uint16_t i=0; i<1024; i++){
                if(pt1[i] != pt2[i]){
                    return false;
                }
            }
        }
    }
    return true;
}

uint32_t get_physical_address(uint32_t* page_dir, uint32_t vaddr){
    uint32_t page_dir_index = vaddr >> 22;
    uint32_t page_table_index = (vaddr >> 12) % 1024;
    printHexInt(((uint32_t*)((page_dir[page_dir_index] & (~7)) + 0xC0000000)));
    return ((uint32_t*)((page_dir[page_dir_index] & (~7)) + 0xC0000000))[page_table_index];
}
void start_process(uint16_t process_id){
    struct process proc = get_process(process_id);
    load_page_table((uint32_t)(proc.page_dir) - 0xC0000000);
    switch_to_user_mode_process(proc.entry_point);
}

