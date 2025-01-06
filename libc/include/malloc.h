struct Block {
    int size;
    int free;
    struct Block* next;
};

void* allocate_block(int bytes, struct Block* iter);
void* malloc(int bytes);
void free(void* addr);
