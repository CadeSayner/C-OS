#include "process.h"
#include "pm.h"
#include "memory.h"
#include "utils.h"

static struct process* proc_list = 0;

static int no_of_processes = 0;

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