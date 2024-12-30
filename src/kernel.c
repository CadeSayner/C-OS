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
    print("Initialising GDT...\r\n");
    initGdt();
    print("GDT successfully initialised.\r\n");
    print("\n");
    print("Initialising IDT...\r\n");
    initIdt();
    print("IDT successfully initialised.\r\n");
    uint32_t mod1 = *(uint32_t*)(bootInfo->mods_addr + 4);
    uint32_t physicalAllocationStart = (mod1 + 0xFFF) & -0xFFF; // just align past the last module that was loaded
    print("\nInitialising Memory @ ");
    printHexInt(physicalAllocationStart);
    print(" ...");

    struct module_struct* modules = (struct module_struct*)(bootInfo->mods_addr);
    // grab module information here for later
    struct Elf32_Ehdr* header = (struct Elf32_Ehdr *) modules[0].module_start;
    uint32_t module_entry = header->e_entry;
    uint32_t module_size = modules[0].module_end - modules[1].module_start;
    uint32_t module_start = modules[0].module_start;
    uint32_t module_end = modules[0].module_end;

    initMemory(physicalAllocationStart);

    print("\n\nMemory initialisation complete!\n\n");

    // testing kread
    // char* echo = kmalloc(10);
    // echo[9] = '\0';
    // kread(echo, 9);
    // print(echo);

    // pass the e_entry and module start and size to a process routine that will create the process
    // descriptor dynamically and then call process start which will then actually execute the slave process
    print("Initialising modules...\n");
    uint16_t process_id = create_proc(module_entry, module_size, module_start, module_end);
    print("Starting sysd...\n");

    // start sysd
    start_process(process_id);

    for(;;);


}