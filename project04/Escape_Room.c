#include <stdio.h>
#include <stdlib.h>


int Is_Number_Correct(int param1, int param2, int param3) {
    
    if(param2 == param1) {
       printf("Number %d: Correct. Nice Job\n", param3);  //Correct Number
       return 1;
    }
        
    printf("Number %d: Incorrect\n", param3);             //Incorrect Number
    return 0; 
}



//First number for the combination:
int f0() {
   return 136182;
}

//Always returns z>>2
int f1() {
    int x = 0x1ac4e47,
        y = 0x270be,
        z = 0x1ffce,
        w = 0x327ed;
    //z += y;

    if(x > z)
      return x==y? x&&y: z>>2;
    else {
        if(y <= z)
            return y;
        else if(y > w)
            return z^w;
        else
            return x/y;
    }
}

int f2(int param1) {
    int x = -1, y = 0;
    while(x++ < 0x15) 
        y += (x*x) + param1;
    return y;
}
void f3(int param1, int *param2) {
    *param2 = *param2+(param1*8);
    *param2 = (*param2 * 1431655766)/2;
}

int f4_helper(int param1) {
   int x = 2;
   return param1*x;
}

int f4(int param1, int param2, int param3) {
    param1 = f4_helper(param1);
    param2 = f4_helper(param2);
    param3 = f4_helper(param3);

    return param1 + param2 + param3;
}

void f5(int *param1) {
    int arr[9] = 
        {0x2484, 0xf75,0xb21, 0x800,0x1d1c, 0x2484,0xf75, 0xb21,0x800};

    //Get the total of arr array and place it in correct number[5]
    *(param1+5) = 0;
    for(int i = 0; i<5; i++)
       *(param1+5) += arr[i];
    
}

int main(int argc, char *argv[]) {
    //printf("%d\n", argc);  
    int *nums = (int*) malloc(6*sizeof(int));
    printf("Enter six numbers: ");

    //Get the six numbers from the user and print it
    int i = 0;
    while(i < 6)
        scanf("%d", &nums[i++]);
    printf("You Entered: %d %d %d %d %d %d\n", 
        nums[0], nums[1], nums[2], nums[3], nums[4], nums[5]);

    //Get the numbers!
    int cN[7];    //sixth position to store numb of correct guesses
    cN[0] = f0(),
    cN[1] = f1(),
    cN[2] = f2(6);
    //printf("%d %d %d ", cN[0], cN[1], cN[2]);
   
    cN[3] = 36566;
    f3(cN[3], &cN[3]);
    cN[4] = f4(0x30f, 0x346, 0x3d7); 

    f5(cN);

    //Iterate through the guesses and check how many were correct
    i = -1;
    while(i++ < 5) {
        if(Is_Number_Correct(nums[i], cN[i], i) == 1)
            nums[6]++;
    }

    if(nums[6] == 6)
        printf("All numbers are correct! Nice work!\n");
    else if(nums[6] == 0)
        printf("You didn't get any correct numbers. Please try again.\n");
    else
        printf("You got %d correct numbers.  Please try again.\n", nums[6]);

    return 0;
}
