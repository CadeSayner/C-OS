#include "vga.h"
#include "gdt.h"

void kmain(void);

void kmain (void){
    Reset();
    print("Welcome to Cade-OS\r\n");

    print("Initialising GDT...\r\n");
    initGdt();
    print("GDT successfully initialised.\r\n");

}