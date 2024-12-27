#include "multiboot.h"

void initMemory(uint32_t memLow);
void dealloc_page_index(uint16_t);
void dealloc_page_vaddr(uint32_t);
uint32_t allocPage(void);
uint32_t alloc_n_consecutive_pages(uint16_t);

struct cache_descriptor{
    struct slab_descriptor *slab_list;
    uint16_t size; // the size of the objects that this cache is meant to store 
};

struct slab_descriptor{
    uint32_t first_obj_address; // starting from the next page assumes contiguous memory for all objects exists for this object
    uint16_t obj_size; // the size of the objects in this slab in bytes
    struct slab_descriptor * next; // the next slab descriptor in this list
    uint8_t objs[4086];
};

void allocate_new_slab_to_cache(struct cache_descriptor* cache_desc);

void* kmalloc(uint32_t bytes);

void kfree(void* vaddr, uint16_t size);

uint32_t next_power_of_2(uint32_t n);

void init_slab_descriptor(struct slab_descriptor* slab_desc, uint16_t size, uint32_t first_obj_vaddr);

extern uint32_t initial_page_dir[1024];