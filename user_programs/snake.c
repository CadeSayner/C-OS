#include "syscalls.h"
#include "stdio.h"

char modname[] __attribute__((section(".modname"), aligned(4))) = "snek";
int main(){
    print("trying to play snake");
    update_echo(0);
    while(1){
        char input = get();
        char str[2];
        str[1] = '\0';
        str[0] = input;
        if(str[0] != '0'){
           // print(str);
        }
    }
}
