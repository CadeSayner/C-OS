#include "stdio.h"
#include "syscalls.h"

void print(void *buf){
    int length = get_length((char*)buf);
    write(buf, length);
}

void cpy(char *buf1, char *buf2, int length){
    for(int i=0; i<length; i++){
        buf2[i] = buf1[i];
    }
}

int get_length(char *buf){  
    int length = 0;
    while(*buf){
        length++;
        buf++;
    }
    return length;
}