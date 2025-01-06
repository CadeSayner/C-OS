#include "stdint.h"

uint16_t create_proc(uint32_t entry, uint32_t size, uint32_t proc_start, uint32_t proc_end, uint32_t data_start, uint32_t proc_off);
void start_process(uint16_t id);

uint32_t get_current_process_brk();
uint32_t set_current_process_brk();
uint32_t get_current_process_sbrk();
uint32_t get_current_process_page_dir();

struct process{
    uint16_t id;
    uint32_t entry_point;
    uint8_t status;
    uint32_t* page_dir;
    struct process* next;
    uint32_t sbrk;
    uint32_t brk;
};
struct process* get_process_ptr(uint16_t id);


#define proc_vaddr_start 0x08048000;

