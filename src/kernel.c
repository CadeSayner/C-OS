#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "multiboot.h"
#include "memory.h"
#include "utils.h"
#include "io.h"
#include "process.h"
#include "terminal.h"

void kmain(uint32_t magic, struct multiboot_info* bootInfo);

void kmain (uint32_t magic, struct multiboot_info* bootInfo){
    Reset();

    vga_set_text_color(COLOR8_LIGHT_BLUE);
    print("_______________________________________\n\n");
    vga_set_text_color(COLOR8_YELLOW);
    print("  Welcome to C-OS\r\n");
    vga_set_text_color(COLOR8_LIGHT_BLUE);
    print("_______________________________________\n");
    vga_set_text_color(COLOR8_LIGHT_GRAY);

    initGdt();
    initIdt();
    
    // get module information from multiboot
    struct module_struct* modules = (struct module_struct*)(bootInfo->mods_addr);

    uint32_t physicalAllocationStart;

    struct module processes[100]; // maximum of 100 concurrent processes allowed in the system, should be defined in a macro rather
    memset(processes, 0, sizeof(processes));
    elf32_parse_modules(processes, modules, bootInfo->mods_count);
    for (uint16_t i = 0; i < 100; i++)
    {
        if(processes[i].module_start == 0){
            physicalAllocationStart = ((processes[i-1].module_end + 0xFFF) & -0xFFF) + 0x1000;
            break;
        }
    }
    initMemory(physicalAllocationStart);

    for (uint16_t i = 0; i < 100; i++)
    {
        if(processes[i].module_start == 0){
            break;
        }
        uint16_t process_id = create_proc(processes[i].module_entry, processes[i].module_size, processes[i].module_start, processes[i].module_end, processes[i].data_start, processes[i].data_offset);
    }

    terminal_start(processes);
    // start sysd
    // start_process(0);
    for(;;);
}