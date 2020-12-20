#include <stdio.h>

int main() {
//    unsigned long long int d = 0xfedcba987654321;
//    int x = d;
//    short y = x;
//    printf("%d, %d, %d, %d\n", sizeof(double), sizeof(unsigned long long int), sizeof(int), sizeof(short) );
//    printf("%llX, %X, %X\n", d, x, y);


//    int x = 0xFFFFFFFF;
//
//    printf("%d, %u", x, (unsigned) x);
    
    int x = -12214;
    printf("%X, %d\n", x, x);
    x = (x>>12) & 0xFF;
    printf("%X, %d\n", x, x);


    return 0;
}
