#include "syscalls.h"
#include "stdio.h"

char modname[] __attribute__((section(".modname"), aligned(4))) = "hello";

void int32_to_hex_string(int num, char* hexString);
void printHexInt(int num){
    char str[] = "           ";
    int32_to_hex_string(num, str);
    print(str);
}

void int32_to_hex_string(int num, char* hexString){
    hexString[0] = '0';
    hexString[1] = 'x';
    const char hexDigits[] = "0123456789ABCDEF";
    const int bitMask = 0xF;
    for(int i = 0; i <= 7; i++){
         hexString[9-i] = hexDigits[(num >> i*4) & bitMask];
    }
}

int main()
{
    print("Hello world!\n");
    int initial_brk = brk(0);
    printHexInt(initial_brk);
    if(brk((void*)(initial_brk + 0x1000))){
        printHexInt (brk(0));
    }else{
        print("failure");
    }
    char answer[] = "               ";
    read(answer, 15);
    print(answer);
    while(1==1){}
    return 0;
}
