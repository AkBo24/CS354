#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* function to show bytes in memory, from location start to start+n*/

void foo(char *x) {
    int buff[1];
    //printf("%x\n",buff[0]);
    strcpy((char*)buff, x);
    printf("%x\n",buff[0]);
}
void callfoo(){
    foo("abcdefghi");
}


void show_mem_rep(char *start, int n)  
{ 
    int i; 
    for (i = 0; i < n; i++) 
         printf(" %.2x", start[i]); 
    printf("\n"); 
} 
  
/*Main function to call above function for 0x01234567*/
int main() 
{ 
   callfoo();
   return 0; 
} 
