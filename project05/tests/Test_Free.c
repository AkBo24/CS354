#include <assert.h> 
#include <stdlib.h> 
#include <stdio.h>
#include "mem.h"

int main () {
    
    assert(Mem_Init(4096,0)==0);
    Mem_Dump();
    void *ptrs[10];

    /*Edge case: one pointer is allocated entire heap*/
    ptrs[0] = Mem_Alloc(4080);
    Mem_Dump();
    assert(ptrs != NULL);
    assert(Mem_Free(ptrs[0]) == 0);

    /*Mem_Free() ret -1 for a pointer not at the beginning of a block*/
    ptrs[1] = Mem_Alloc(10);
    printf("Alloc ptrs[1]\n");
    Mem_Dump();
    assert(Mem_Free(ptrs[1])==0);
    printf("Free ptrs[1]\n");
    Mem_Dump();
    assert(Mem_Free(ptrs[1])==-1);
   
    /*Testing top neighbor coalescing*/
    assert((ptrs[1] = Mem_Alloc(100)) != NULL);
    Mem_Dump();
    assert((ptrs[2] = Mem_Alloc(50)) != NULL);
    
    assert(Mem_Free(ptrs[1]) == 0);
    assert(Mem_Free(ptrs[2]) == 0);
}
