#include "utils.h"
#include "vga.h"

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