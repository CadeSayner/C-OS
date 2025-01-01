#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "memory.h"
#include "utils.h"
#include "io.h"
#include "process.h"

void kmain(uint32_t magic, struct multiboot_info* bootInfo);

void kmain (uint32_t magic, struct multiboot_info* bootInfo){
    Reset();
    print("Welcome to Cade-OS\r\n");
    initGdt();
    initIdt();

    
    uint32_t mod1 = *(uint32_t*)(bootInfo->mods_addr + 4);
    uint32_t physicalAllocationStart = (mod1 + 0xFFF) & -0xFFF; // just align past the last module that was loaded

    // get module information from multiboot
    struct module_struct* modules = (struct module_struct*)(bootInfo->mods_addr);

    struct module processes[100]; // maximum of 100 concurrent processes allowed in the system, should be defined in a macro rather
    memset(processes, 0, sizeof(processes));
    elf32_parse_modules(processes, modules, bootInfo->mods_count);
    
    initMemory(physicalAllocationStart);

    // testing kread
    // char* echo = kmalloc(10);
    // echo[9] = '\0';
    // kread(echo, 9);
    // print(echo);

    print("Initialising modules...\n");

    for (uint16_t i = 0; i < 100; i++)
    {
        if(processes[i].module_start == 0){
            break;
        }
        uint16_t process_id = create_proc(processes[i].module_entry, processes[i].module_size, processes[i].module_start, processes[i].module_end);
    }
    
    print("Starting sysd...\n");

    // start sysd
    start_process(0);
    for(;;);
}