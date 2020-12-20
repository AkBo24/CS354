#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main () {
    assert(Mem_Init(4096,0)==0);
    void *ptrs[10];
    Mem_Dump();

    
    ptrs[0] = Mem_Alloc(100);
    Mem_Dump();
    
    ptrs[1] = Mem_Alloc(100);
    Mem_Dump();
    
    ptrs[2] = Mem_Alloc(100);
    Mem_Dump();
    //ptrs[1] = Mem_Alloc(100);
    //Mem_Dump();
    //assert();
}
