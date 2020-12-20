#include <stdio.h>
#include <string.h>

int square(int big, int temp) {
    int i;
    i = big;
       if (big > 10) i=i%10;
    return (i*temp); 
}

int mult(int base, int mul) {
    //return(base == 0)? 0: mult(base-1, mul) + mul;
    return mult(base-1, mul) + mul;

}

int main() {
    int sum, i;
    char string[120];

    sum=0;
    strcpy(string,"Hello!");

    for (i=1;i<10;i++) {
        sum+=i;
        sum=square(sum,i);
    }
    printf("sum=%d\n",sum);
    int num = mult(4, 3);
    printf("%d\n", num);


    return 0;
}
