#include "vga.h"
#include "gdt.h"
#include "idt.h"
void kmain(void);

void kmain (void){
    Reset();
    print("Welcome to Cade-OS\r\n");

    print("Initialising GDT...\r\n");
    initGdt();
    print("GDT successfully initialised.\r\n");
    print("\n");
    print("Initialising IDT...\r\n");
    initIdt();
    print("IDT successfully initialised.\r\n");
    print(1/0);

}