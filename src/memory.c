#include "stdint.h"
#include "multiboot.h"
#include "vga.h"
#include "memory.h"
#include "utils.h"

static uint32_t initialAllocationFrame;
static uint8_t mmap[30000]; //  ~120mb of memory mapped 
static struct cache_descriptor default_cache;

static struct cache_descriptor general_caches[8];

void initMemory(uint32_t memLow){
    // invalidate the first PTE, since we no longer need it
   initial_page_dir[0] = 0;
   invalidate(0);

   initialAllocationFrame = (memLow / 4096) + 5;
   print("The initial page frame is\n");
   printHexInt(initialAllocationFrame);
   memset(mmap, 0, sizeof(mmap));

   for (uint8_t i = 0; i < 8; i++)
   {
     general_caches[i].size = pow(2, i);
     general_caches[i].slab_list = 0;
   }
}

void* kmalloc(uint32_t bytes){
    uint32_t next_pwr_2 = next_power_of_2(bytes);
    struct cache_descriptor cache_desc = general_caches[calc_index(next_pwr_2)];
    if(cache_desc.slab_list == 0){
        print("\n Allocating a slab to the cache\n");
        allocate_new_slab_to_cache(general_caches + calc_index(next_pwr_2));
    }
    struct slab_descriptor* curr = cache_desc.slab_list;
    while(curr != 0){
        for(uint16_t i = 0; i < 4086; i++){
            if((curr->objs)[i] == 0){
                (curr->objs)[i] = 1;
                // return the address that we need here
                return (void *)(curr->first_obj_address + i * cache_desc.size);
            }
        }
        curr = curr->next;
    }
    // havent found anything so allocate a new slab to the cache 
    allocate_new_slab_to_cache(&cache_desc);
    // just recurse for now since we know that it should come right now
    // TODO Can be improved significantly by actually do the allocation here since we have the curr pointer
    // already in the right place so can bypass the allocation
    return kmalloc(bytes);
    // curr points to the slab descriptor at the end of the list
}

// frees the memory at vaddr of the provided size
void kfree(void* vaddr, uint16_t size){
    uint32_t next_pwr_2 = next_power_of_2(size);
    // a copy of the cache_desc for the purposes of extracting relevant data later
    struct cache_descriptor cache_desc = general_caches[calc_index(next_pwr_2)];

    struct slab_descriptor* curr = cache_desc.slab_list;
    while(curr != 0){
        for(uint16_t i = 0; i < 4086; i++){
            if(curr->first_obj_address <= vaddr && vaddr < curr->first_obj_address +  (4086*cache_desc.size)){
                // the address falls within the range of this slab
                // now calculate the index within the array
                uint16_t rm_index = ((uint32_t)vaddr - curr->first_obj_address)/cache_desc.size;
                curr->objs[rm_index] = 0; // deallocate that object
            }        
        }
        curr = curr->next;
    }
}

int calc_index(int pwr2){
    int count = 0;
    while(pwr2 != 0){
        pwr2 >>= 1;
        count++;
    }
    return count-1;
}

// return the smallest power of 2 larger than n
uint32_t next_power_of_2(uint32_t n) {
    if (n == 0) {
        return 1;  // 0 is a special case, return 1 as the next power of 2
    }
    
    n--;  // Decrement to handle the case where n is already a power of 2
    n |= n >> 1;  // Spread the highest set bit
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;  // For 32-bit systems, handle up to 32 bits
    return n + 1;  // Increment to get the next power of 2
}

// return the linear address of a page allocated in kernel space
uint32_t allocPage(){
    for (uint16_t i = 0; i < 30000; i++)
    {
        if(mmap[i] == 0){
            mmap[i] = 1;
            return (uint32_t)( 0xC0000000 + (initialAllocationFrame + i)*4096);
        }
    }
}

// deallocate a page with a specified index
void dealloc_page_index(uint16_t i){
    mmap[i] = 0;
}

//deallocate n page frames starting from the frame at some linear vaddr
void dealloc_n_pages(uint32_t vaddr, uint16_t n){
    uint16_t initial_frame = (vaddr-0xC0000000)/4096;
    for(int i = initial_frame; i < i + n; i++){
        mmap[i] = 0;
    }
}

void dealloc_page_vaddr(uint32_t vaddr){
    mmap[(vaddr-0xC0000000)/4096] = 0;
}

// allocates a new slab to a cache of the appropriate size
void allocate_new_slab_to_cache(struct cache_descriptor * cache_desc){
    struct slab_descriptor* cur = cache_desc->slab_list;
    // cur points to the last slab descriptor that we have
    // create a new slab descriptor by first requesting memory from the allocator and then using the first page for the slab_descriptor
    struct slab_descriptor* new_slab = alloc_n_consecutive_pages(((cache_desc->size * 4086) / 4096) + 2);
    // printHexInt(new_slab);
    init_slab_descriptor(new_slab, cache_desc->size, (uint32_t)new_slab + 4096);
    if(cache_desc->slab_list == 0){
        cache_desc->slab_list = new_slab;
    }else{
        while(cur->next != 0){
            cur = cur->next;
        }
        cur->next = new_slab;
    }
}

void init_slab_descriptor(struct slab_descriptor* slab_desc, uint16_t size, uint32_t first_obj_vaddr){
    slab_desc->first_obj_address = first_obj_vaddr;
    slab_desc->obj_size = size;
    memset(slab_desc->objs, 0, slab_desc->obj_size * 4086); // initialise the slab array to a bunch of zeroes
    slab_desc->next = 0; // point nowhere initially
}

// allocates n consecutive pages in kernel space and returns the linear address of the first page
// if allocation was unsuccessful the integer 0xFFFFFFFF is returned instead
uint32_t alloc_n_consecutive_pages(uint16_t n){
    // print("allocating");
    // printHexInt(n);
    // print("consecutive pages\n");
    uint16_t current_cand_index = 0;
    uint16_t current_index = 0;
    while(current_index - current_cand_index != n && current_index < 30000){
        if(mmap[current_index] != 0){
            current_cand_index = current_index + 1;
        }
        current_index++;
    }
    if(current_index == 30000){
        // we can assume that allocation was unsuccessful
        return 0xFFFFFFFF;
    }else{
        for (uint16_t i = current_cand_index; i < current_cand_index + n; i++)
        {
            // allocate the memory to the kernel
            mmap[i] = 1;
        }
        return (uint32_t)( 0xC0000000 + (initialAllocationFrame + current_cand_index)*4096);
    }
}

void invalidate(uint32_t vaddr){
    asm volatile ("invlpg %0" :: "m"(vaddr));
}