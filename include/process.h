#include "stdint.h"

uint16_t create_proc(uint32_t entry, uint32_t size, uint32_t proc_start, uint32_t proc_end);
void start_process(uint16_t id);
struct process{
    uint16_t id;
    uint32_t entry_point;
    bool running;
    uint32_t* page_dir;
    struct process* next;
};

