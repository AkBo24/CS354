/* a few allocations in multiples of 4 bytes followed by frees */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "mem.h"

int main() {
   assert(Mem_Init(4096,0) == 0);
   Mem_Dump();
   void* ptr[4];

   printf("Alloc 0,1\n");
   ptr[0] = Mem_Alloc(4);
   ptr[1] = Mem_Alloc(8);
   Mem_Dump();

   printf("Free 0,1\n");
   assert(Mem_Free(ptr[0]) == 0);
   Mem_Dump();
   assert(Mem_Free(ptr[1]) == 0);
   Mem_Dump();

   printf("Alloc 2,3\n");
   ptr[2] = Mem_Alloc(16);
   ptr[3] = Mem_Alloc(4);
   Mem_Dump();

   assert(Mem_Free(ptr[2]) == 0);
   assert(Mem_Free(ptr[3]) == 0);

   assert(Mem_Free((void*)0x28ff44) == -1);

   exit(0);
}
