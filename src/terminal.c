#include "multiboot.h"
#include "vga.h"
#include "utils.h"
#include "process.h"
#include "terminal.h"

struct module* tm_processes;
uint32_t process_count;

void terminal_start(struct module processes[100]){
    tm_processes = processes; 
    process_count = get_process_count();
    print("\n\nModules loaded:\n\n");
    vga_set_text_color(COLOR8_LIGHT_BLUE);
    printModules();
    vga_set_text_color(COLOR8_LIGHT_GRAY);
    while(1){
        print("\nEnter pid to run:\n");
        char* input = kmalloc(81); // maximum number of bytes needed to store the entire line
        input[81] = '\0';
        kread(input, 80); // read the line into input
        print("\n");
        uint32_t index = parse_uint(input);
        if(index <= process_count -1){
            // begin the process they entered
            start_process(index);
            break;
        }else{
            print("invalid entry");
        }
    }
}

uint32_t get_process_count(){
    uint32_t count = 0;
    for (uint16_t i = 0; i < 100; i++)
    {
        if(tm_processes[i].module_start == 0){
            break;
        }
        count++;
    }
    return count;
}

void printModules(){
    for (uint16_t i = 0; i < 100; i++)
    {
        if(tm_processes[i].module_start == 0){
            break;
        }
        print(">> ");
        print(tm_processes[i].module_name);
        vga_set_text_color(COLOR8_LIGHT_GREEN);
        print(" (pid=");
        print_dec_uint(i);
        print(")\n");

        vga_set_text_color(COLOR8_LIGHT_BLUE);
    }
}