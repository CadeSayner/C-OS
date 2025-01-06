#include "malloc.h"
#include "stdio.h"

int cur_brk = 1;
struct Block* blk_hd = 1 ;

void int32_to_hex_string_here(int num, char* hexString);
void printHexInt_h(int num){
    char str[] = "           ";
    int32_to_hex_string_here(num, str);
    print(str);
}

void int32_to_hex_string_here(int num, char* hexString){
    hexString[0] = '0';
    hexString[1] = 'x';
    const char hexDigits[] = "0123456789ABCDEF";
    const int bitMask = 0xF;
    for(int i = 0; i <= 7; i++){
         hexString[9-i] = hexDigits[(num >> i*4) & bitMask];
    }
}

void* malloc(int bytes){
    if(blk_hd == 1){
        // this is all a bunch of bs to skirt around mapping in bss
        cur_brk = brk(0);
        return allocate_block(bytes, 0);
    }
    else{
        struct Block* iter = blk_hd;
        while(iter->next != 0){
            if(iter->size == bytes){
               return (void*)((int)iter + sizeof(struct Block));
            }
            iter = iter->next;
        }
        // at the end of the list we can add it to the last block
        return allocate_block(bytes, iter);
    }
}

void* allocate_block(int bytes, struct Block* iter){
    brk(cur_brk + bytes + sizeof(struct Block));
    int alloc_start = cur_brk; // keep a copy of the cur_brk for allocation purposes
    update_brk();

    if(brk(0) == alloc_start+ bytes + sizeof(struct Block)){
        // memory allocation was sucessful
        init_blk((struct Block*)alloc_start, bytes, 1, 0);
        if(iter != 0){
            iter->next = alloc_start;
        }
        return (void*)(alloc_start+sizeof(struct Block));
    }else{
        return 0;
    }
}

void update_brk(){
    cur_brk = brk(0);
}

void init_blk(struct Block* blk, int size, int free, struct Block* next){
    blk->size = size;
    blk->free = free;
    blk->next = next;
}