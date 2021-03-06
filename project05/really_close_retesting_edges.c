/******************************************************************************
 * FILENAME: mem.c
 * AUTHOR:   cherin@cs.wisc.edu <Cherin Joseph>
 * DATE:     20 Nov 2013
 * PROVIDES: Contains a set of library functions for memory allocation
 * *****************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

int fit, max = -1;

/* this structure serves as the header for each block */
typedef struct block_hd{
  /* The blocks are maintained as a linked list */
  /* The blocks are ordered in the increasing order of addresses */
  struct block_hd* next;

  /* size of the block is always a multiple of 4 */
  /* ie, last two bits are always zero - can be used to store other information*/
  /* LSB = 0 => free block */
  /* LSB = 1 => allocated/busy block */

  /* So for a free block, the value stored in size_status will be the same as the block size*/
  /* And for an allocated block, the value stored in size_status will be one more than the block size*/

  /* The value stored here does not include the space required to store the header */

  /* Example: */
  /* For a block with a payload of 24 bytes (ie, 24 bytes data + an additional 8 bytes for header) */
  /* If the block is allocated, size_status should be set to 25, not 24!, not 23! not 32! not 33!, not 31! */
  /* If the block is free, size_status should be set to 24, not 25!, not 23! not 32! not 33!, not 31! */
  int size_status;

}block_header;

/* Global variable - This will always point to the first block */
/* ie, the block with the lowest address */
block_header* list_head = NULL;


/* Function used to Initialize the memory allocator */
/* Not intended to be called more than once by a program */
/* Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated 
   	      policy: indicates the policy to use eg: best fit is 0*/
/* Returns 0 on success and -1 on failure */
int Mem_Init(int sizeOfRegion,  int policy)
{
  int pagesize;
  int padsize;
  int fd;
  int alloc_size;
  void* space_ptr;
  static int allocated_once = 0;
  
  if(0 != allocated_once)
  {
    fprintf(stderr,"Error:mem.c: Mem_Init has allocated space during a previous call\n");
    return -1;
  }
  if(sizeOfRegion <= 0)
  {
    fprintf(stderr,"Error:mem.c: Requested block size is not positive\n");
    return -1;
  }

  /* Get the pagesize */
  pagesize = getpagesize();

  /* Calculate padsize as the padding required to round up sizeOfRegion to a multiple of pagesize */
  padsize = sizeOfRegion % pagesize;
  padsize = (pagesize - padsize) % pagesize;

  alloc_size = sizeOfRegion + padsize;

  /* Using mmap to allocate memory */
  fd = open("/dev/zero", O_RDWR);
  if(-1 == fd)
  {
    fprintf(stderr,"Error:mem.c: Cannot open /dev/zero\n");
    return -1;
  }
  space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  if (MAP_FAILED == space_ptr)
  {
    fprintf(stderr,"Error:mem.c: mmap cannot allocate space\n");
    allocated_once = 0;
    return -1;
  }
  
  allocated_once = 1;
  
  /* To begin with, there is only one big, free block */
  list_head = (block_header*)space_ptr;
  list_head->next = NULL;
  /* Remember that the 'size' stored in block size excludes the space for the header */
  list_head->size_status = alloc_size - (int)sizeof(block_header);
  fit = policy;
  return 0;
}


/* Function for allocating 'size' bytes. */
/* Returns address of allocated block on success */
/* Returns NULL on failure */
/* Here is what this function should accomplish */
/* - Check for sanity of size - Return NULL when appropriate */
/* - Round up size to a multiple of 4 */
/* - Traverse the list of blocks and allocate the best free block which can accommodate the requested size */
/* -- Also, when allocating a block - split it into two blocks when possible */
/* Tips: Be careful with pointer arithmetic */
void* Mem_Alloc(int size)
{
  /* Your code should go in here */

  max = list_head->size_status>max?list_head->size_status:max;
  if(size <= 0 || size > max) return NULL;
  size += (size%4) + sizeof(block_header);                      //round size to closest multiple of 4
  
  /*Find block based based on policy, 0 = best, 1 = first, 2 = worst*/
  block_header *curr = list_head;
  int total = 0;

  if(!fit) {                                                    //best fit 
    //dummy block for a temporary best/worst find block
    block_header dummy = {.next=NULL, .size_status=max+1}, *best = &dummy;
    int busy;

    while(curr != NULL) {
        busy = (curr->size_status&1 || 
                 (!(curr->size_status&1) && size>curr->size_status));
        //Update best only if curr is closer to the requested size
        if(!busy && (curr->size_status-size < best->size_status-size)) 
            best = curr;
        curr = curr->next;               
    }
    //check if traversed entire list then update curr pointer to the best found free spot
    if(best==&dummy) return NULL; 
    curr = best;
  }
  else if(fit == 1) {                                           //first fit
    total += curr->size_status;
    while(curr != NULL && (curr->size_status&1 || 
               (!(curr->size_status&1) && size>curr->size_status)))
            curr = curr->next;
    if(!curr) return NULL;
  }
  else if(fit == 2){                                            //worst fit
    block_header dummy = {.next = NULL, .size_status = -1}, *worst = &dummy;
    int busy;

    while(curr != NULL) {
        busy = (curr->size_status&1 || 
                 (!(curr->size_status&1) && size>curr->size_status));
        //Update worst only if curr is larger than both current worst and req size
        if(!busy && (curr->size_status-size > worst->size_status-size))
            worst = curr;
        curr = curr->next;
    }
    if(worst == &dummy) return NULL;
    curr = worst;
  }
  else {                                                        //not a valid policy
    return NULL;
  }

  /*
   * If found block is larger than allocated size 
   * and valid (to 4 byte alignment), split
  */
  int split_size = curr->size_status-size, new_addr = size%8!=0? size/8+1 : size/8;
  if(split_size > 0) {
    block_header *free = curr+new_addr+1;
    
    // Split current block and assign a new block to the free space
    free->next = curr->next; free->size_status = split_size-sizeof(block_header);
    curr->next = free; curr->size_status = size-sizeof(block_header)+1;
    //Mem_Dump();
    return curr+1;
  }
  else if (split_size == 0) {                                  //found block is perfect size
    curr->size_status++;
    return curr;
  }
  printf("huston, we have a problem");
  return NULL;
}

/* Function for freeing up a previously allocated block */
/* Argument - ptr: Address of the block to be freed up */
/* Returns 0 on success */
/* Returns -1 on failure */
/* Here is what this function should accomplish */
/* - Return -1 if ptr is NULL */
/* - Return -1 if ptr is not pointing to the first byte of a busy block */
/* - Mark the block as free */
/* - Coalesce if one or both of the immediate neighbours are free */
int Mem_Free(void *ptr)
{
  /* Your code should go in here */
  if(ptr == NULL) return -1;

  block_header *curr = list_head;
  while(curr!=NULL) {
    //Mem_Dump();
    //curr += 1;
    //printf("ptr: %p curr: %p curr-1: %p\n", ptr, curr, curr+1);
    if(ptr == curr+1) {
        curr->size_status--;                                            //marking pointer as free
        if(curr+curr->size_status/8+1 == list_head+max/8+1) return 0;   //Edge case: ptr is entire heap

        block_header *end = curr+curr->size_status/8+1;                 //create end tag
        end->size_status  = curr->size_status; end->next = curr;

        /*Check if bottom neighbor is free, coalesce if so*/
        if(curr->next && !(curr->next->size_status&1)){
            //add size of bottom header to curr, and include header
            curr->size_status += (end+1)->size_status+sizeof(block_header);
            curr->next = curr->next->next; //remove the next block from the list
        }

        /*Check if top neighbor is free, coalesce if so*/
        if(curr == list_head) return 0;                                 //Edge case ptr is top of heap
        block_header *prev = curr-1;
        if(prev && !(prev->size_status&1)) {
            prev = prev-prev->size_status/8-1;                          //reassign prev to the head of its block
            prev->size_status += curr->size_status+sizeof(block_header);
            prev->next = curr->next;
        }

        return 0;
    }
    curr = curr->next;
  }

  //pointer does not belong to list, most likely not the first byte of a block
  return -1; 
}

/* Function to be used for debug */
/* Prints out a list of all the blocks along with the following information for each block */
/* No.      : Serial number of the block */
/* Status   : free/busy */
/* Begin    : Address of the first useful byte in the block */
/* End      : Address of the last byte in the block */
/* Size     : Size of the block (excluding the header) */
/* t_Size   : Size of the block (including the header) */
/* t_Begin  : Address of the first byte in the block (this is where the header starts) */
void Mem_Dump()
{
  int counter;
  block_header* current = NULL;
  char* t_Begin = NULL;
  char* Begin = NULL;
  int Size;
  int t_Size;
  char* End = NULL;
  int free_size;
  int busy_size;
  int total_size;
  char status[5];

  free_size = 0;
  busy_size = 0;
  total_size = 0;
  current = list_head;
  counter = 1;
  fprintf(stdout,"************************************Block list***********************************\n");
  fprintf(stdout,"No.\tStatus\tBegin\t\tEnd\t\tSize\tt_Size\tt_Begin\n");
  fprintf(stdout,"---------------------------------------------------------------------------------\n");
  while(NULL != current)
  {
    t_Begin = (char*)current;
    Begin = t_Begin + (int)sizeof(block_header);
    Size = current->size_status;
    strcpy(status,"Free");
    if(Size & 1) /*LSB = 1 => busy block*/
    {
      strcpy(status,"Busy");
      Size = Size - 1; /*Minus one for ignoring status in busy block*/
      t_Size = Size + (int)sizeof(block_header);
      busy_size = busy_size + t_Size;
    }
    else
    {
      t_Size = Size + (int)sizeof(block_header);
      free_size = free_size + t_Size;
    }
    End = Begin + Size;
    fprintf(stdout,"%d\t%s\t0x%08lx\t0x%08lx\t%d\t%d\t0x%08lx\n",counter,status,(unsigned long int)Begin,
           (unsigned long int)End,Size,t_Size,(unsigned long int)t_Begin);
    total_size = total_size + t_Size;
    current = current->next;
    counter = counter + 1;
  }
  fprintf(stdout,"---------------------------------------------------------------------------------\n");
  fprintf(stdout,"*********************************************************************************\n");

  fprintf(stdout,"Total busy size = %d\n",busy_size);
  fprintf(stdout,"Total free size = %d\n",free_size);
  fprintf(stdout,"Total size = %d\n",busy_size+free_size);
  fprintf(stdout,"*********************************************************************************\n");
  fflush(stdout);
  return;
}




