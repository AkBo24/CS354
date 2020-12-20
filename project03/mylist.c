#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INTS 1000
#define MAX_STRL 65535

const char *USAGE = "Usage: ./mylist <-i|-d> <OUTPUT_FILE> <FILE>...";
const char *BADFILE = "ERROR: unable to process file ";
const char *LSTRECV = "LIST CONTENT AS OF FILE %s: %s\n";
const char *ORIGLST = "ORIGINAL LIST: %s\n";
const char *SORTEDLST = "SORTED LIST: %s\n";

typedef struct node {
    int data;
    struct node *next;
} node_t;

char * list_to_string(node_t *head) {
    char *s = calloc(MAX_STRL, sizeof(char));
    if (head->next == NULL) {
        strcat(s, "Linked List is Empty.");
    } else {
        node_t *curr = head->next;
        strcat(s, "head->");
        while (curr != NULL) {
            char tempstr[12];
            sprintf(tempstr, "|%d|->", curr->data);
            strcat(s, tempstr);
            curr = curr->next;
        }
        strcat(s, "NULL");
    }
    return s;
}

void insert(node_t *head, int data) {
    node_t *new_node = (struct node*) malloc(sizeof(node_t));
    new_node->data = data; new_node->next = head->next;

    head->next = new_node;
    return;
}

ssize_t build_list_from_file(node_t *head, const char *filename) { 
    ssize_t numints = 0;

    //Validate the current file exists
    FILE *input = fopen(filename, "r");
    if(input == NULL) {
       printf("%s%s\n", BADFILE, filename);
       return -1;
    }

    //Get the a line of integers and store into *line
    char *line = NULL;
    ssize_t read;
    size_t buffSize = 1000*sizeof(char); //maximum possible number of integers
    if( (read = getline(&line, &buffSize, input)) == -1) {
        printf("%s%s\n", BADFILE, filename);
        return -1;
    }

    //tokenize *line
    char lineTok[strlen(line)]; strcpy(lineTok, line); //store line into a char array for tokenization
    line = strtok(lineTok, "| ");

    while(line != NULL && (*line > 47 && *line < 58)) {
        numints += sizeof(int);
        insert(head, atoi(line));
        //printf("Line: %d\n", *line);
        line = strtok(NULL, "| ");
    }

    //subtract one sizeof int since while loop loops one more time than end of line
    fclose(input);
    return numints; 
}

void list_to_array(int *integers, node_t *head) {
    //printf("%d\n", head->data);
    node_t *curr = head;
   
    //Iterate through the linked list and add each data to the next pos of integer
    int i = 0;
    while( curr != NULL && (curr = curr->next))
        integers[i++] = curr->data;

    return;
}

int qsort_inc(const void *p1, const void *p2) {
    return (*(int*)p2 - *(int*)p1);
}

int qsort_dec(const void *p1, const void *p2) {
    return (*(int*)p1 - *(int*)p2);
}

void array_to_list(node_t *head, int *integers, size_t len) {
    //printf("METHOD LEN: %d", len);
    for(int i = 0; i < len; i++)
        insert(head, integers[i]);
    return;
}

int write_to_file(const char *filename, node_t *orighead, node_t *newhead) {
    FILE *write = fopen(filename, "w");
    if(write == NULL)
        printf("%s%s\n", BADFILE, filename);

    fprintf(write, "ORIGINAL LIST: %s\n", list_to_string(orighead));
    fprintf(write, "SORTED LIST: %s\n", list_to_string(newhead));

    fclose(write);
    return 0;
}


int main(int argc, char *argv[]) {
    node_t head = {.data = 0, .next = NULL};

    // Verify and process command line arguments
    char f2 = *(argv[1]+1);
    if(argc < 4) {
        printf("%s\n", USAGE);
        exit(1);
    }
    else if(f2 != 'i' && f2 != 'd') {
       printf("%c, %s\n", f2, USAGE);
        exit(1);
    }

    // Read from files and generate a list
    int totNums = 0, fileNum = 0;
    for(int i = 3; i < argc; i++) {
        fileNum = build_list_from_file(&head, argv[i])/sizeof(int);
        if(fileNum < 0 || fileNum > MAX_INTS)  continue; //File was not correctly formatted
       
        totNums+= fileNum; //add to the total number of numbers
        printf("LIST CONTENT AS OF FILE %s: %s\n", argv[i], list_to_string(&head));
    }

    // Convert the list to an array
    int listArr[totNums];
    list_to_array(listArr, &head);

    // Sort the array
    if(f2 == 'i')
        qsort(listArr, totNums, sizeof(int), qsort_inc);
    else
        qsort(listArr, totNums, sizeof(int), qsort_dec);
    
    // Convert the sorted array to a new list
    node_t sorted = {.data = 0, .next = NULL};
    //printf("fileNums; %d, TOTNUMS: %d\n", fileNum, totNums);
    array_to_list(&sorted, listArr, totNums);

    // Write both lists to an output file
    write_to_file(argv[2], &head, &sorted);
    return 0;
}
