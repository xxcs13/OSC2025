#include "allocator.h"

static char *top = (char *)MEM_START;                  // Record the current available location of the HEAP
static char *end = (char *)(MEM_START + MEM_SIZE);     // Maximum range of the heap


// initialize the allocator
void init_allocator() {
    top = (char *)MEM_START;
    end = (char *)(MEM_START + MEM_SIZE);
}

void* simple_alloc(size_t size) {
    // Maybe alignment is not necessary.
    // align the top to 8 bytes
    top = (char *)(((unsigned long)top + 7) & ~7);
    
    // align the size to 8 bytes
    size = (size + 7) & ~7;
    
    if(top + size > end)
        return NULL;
        
    top += size;
    return top;
}
