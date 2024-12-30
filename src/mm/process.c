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

void start_process(uint16_t process_id){
    // assume that it is the 0th process for now
    load_page_table((uint32_t)(proc_list->page_dir) - 0xC0000000);
    print("okay");
    printHexInt(proc_list->entry_point);
    switch_to_user_mode_process(proc_list->entry_point);
    //switch_to_user_mode();
    // give the page table to the cpu:
}

void switch_to_user_mode()
{
   // Set up a stack structure for switching to user mode.
   asm volatile("  \
     cli; \
     mov $0x23, %ax; \
     mov %ax, %ds; \
     mov %ax, %es; \
     mov %ax, %fs; \
     mov %ax, %gs; \
                   \
     mov %esp, %eax; \
     pushl $0x23; \
     pushl %eax; \
     pushf; \
     pushl $0x1B; \
     push $1f; \
     iret; \
   1: \
     ");
} 