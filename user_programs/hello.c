#include "syscalls.h"

char modname[] __attribute__((section(".modname"), aligned(4))) = "hello";

int main2()
{
    char input[] = "Hello world!";
    write(input, sizeof(input));
    char answer[] = "               ";
    read(answer, 15);
    write(answer, 15);
    while(1==1){}
    return 0;
}

