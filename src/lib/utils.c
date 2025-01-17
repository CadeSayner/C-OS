#include "utils.h"
#include "vga.h"
#include "memory.h"

void memset(void *dest, char val, uint32_t count){
    char *temp = (char*) dest;
    for(; count!=0; count--){
        *(temp++) = val;
    }
}

void outPortB(uint16_t port, uint8_t value){
    // pushes a byte of data to an IO port
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ __volatile__(
        "inb %1, %0"  // inb instruction
        : "=a"(value)  // Output: value will be placed in 'value' variable
        : "d"(port)    // Input: port address
    );
    return value;
}

void int32_to_hex_string(uint32_t num, char* hexString){
    hexString[0] = '0';
    hexString[1] = 'x';
    const char hexDigits[] = "0123456789ABCDEF";

    const int bitMask = 0xF;
    for(int i = 0; i <= 7; i++){
         hexString[9-i] = hexDigits[(num >> i*4) & bitMask];
    }
}

void printHexInt(uint32_t num){
    char str[] = "          ";
    int32_to_hex_string(num, str);
    print(str);
}

uint32_t pow(int n, int m){
    if(m == 0){
        return 1;
    }
    uint32_t num = 1;
    for (int i = 0; i < m; i++)
    {
        num *= n;
    }
    return num;
}


void elf32_parse_modules(struct module* processes, struct module_struct* modules, int mod_count){
    for (uint16_t i = 0; i < mod_count; i++)
    {
        // grab module information here for later
        struct Elf32_Ehdr* header = (struct Elf32_Ehdr *) modules[i].module_start;
        uint32_t module_entry = header->e_entry;
        uint32_t module_size = modules[i].module_end - modules[i].module_start;
        uint32_t module_start = modules[i].module_start;
        uint32_t module_end = modules[i].module_end;

        struct Elf32_Shdr *section_headers = (struct Elf32_Shdr *)((char *)header + header->e_shoff);

        // Get the section string table offset from the ELF header (it's typically at e_shstrndx)
        char *section_string_table = (char *)header + section_headers[header->e_shstrndx].sh_offset;
        uint32_t data_start;
        uint32_t data_offset;
        for (int j = 0; j < header->e_shnum; j++) {
            // Get the name of the section by looking up the string table entry
            char *section_name = section_string_table + section_headers[j].sh_name;
            if(section_name[1]=='d' && section_name[2]=='a'){
                data_start = section_headers[j].sh_addr;
                data_offset = section_headers[j].sh_offset;
            }
            // Print out the section name and its address
            if(section_name[1] == 'm' && section_name[2]=='o'){
                char*module_name = (char*)header + section_headers[j].sh_offset;
                struct module proc;
                proc.module_entry = module_entry;
                proc.module_end = module_end;
                proc.module_size = module_size;
                proc.module_start = module_start; 
                proc.module_name = (char*)((uint32_t)module_name + 0xC0000000);
                processes[i] = proc;
            };
        }
        processes[i].data_start = data_start;
        processes[i].data_offset = data_offset;
    }
}

int32_t parse_uint(const char *s){
    uint32_t res = 0;
    while(*s){
        if(*s >= 48 && *s <= 57){
            int32_t digit = (*s) - 48;
            res = res*10 + digit;
            s++;
        }else{
            return -1;
        }
    }
    return res;
}

void print_dec_uint(uint32_t num){
    if(num == 0){
        print("0");
        return;
    }
    char decimal_digits[] = "0123456789";
    int length = 0;
    uint32_t cpy = num;

    while(cpy != 0){
        cpy/=10;
        length++;
    }
    char *dec_string = kmalloc(length + 1);
    dec_string[length] = '\0';
    for (uint16_t i = 0; i < length; i++)
    {
        dec_string[length-i-1] = decimal_digits[num % 10];
        num/=10;
    }
    print(dec_string);
}
