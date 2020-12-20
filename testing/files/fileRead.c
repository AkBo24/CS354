#include <stdio.h>
#include <stdlib.h>

#define MAX 1

int main() {

    FILE *fp;
    char buff[MAX];

    fp = fopen("readme.txt", "r");

    while(fgets(buff, MAX, fp)) {
        printf("%s  ", buff);
    }

    printf("\n");
    fclose(fp);

    return 0;
}
