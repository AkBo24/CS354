#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max 100

int main() {

    FILE *fp;
    char buff[max];
    char *str = "Writing to file using fwrite...";

    fp = fopen("writeMe.txt", "w+");
    if(!fp) exit(1);

    /* Write data to the file */
    fwrite(str, strlen(str) + 1, sizeof(char), fp);

    /*Seek to beggining of file*/
    fseek(fp, 0, SEEK_SET);

    /* Read from file */
    fread(buff, strlen(str), 1, fp);
    printf("%s\n", buff);
    





    ////Writing to a file   
    //fputs("Writing to file....", fp);

    //fclose(fp);

    //fp = fopen("writeMe.txt", "r");
    ////Reading to a file
    //while(fgets(buff, max, fp))
    //    printf("%s", buff);

    fclose(fp);
    return 0;
}
