#include <stdio.h>

// avoid hardcoding with numeric constants for array lengths
#define MAX_BINARY_STRING_LENGTH 33
#define MAX_HEXADECIMAL_STRING_LENGTH 9

// function declarations (prototypes)
// do not change these
int Get_Number_From_User();
void Convert_Number_To_Binary(const int num,char *binary);
void Convert_Number_To_Hexadecimal(const int num, const char *binary, char *hexadecimal);
void getHexChar(const char *bin, char *const hexChar);
int power(int base, int pow);

// you do not need to change main
// feel free to add additional printf statments while debugging 
// but remove them before submitting
// this is know as scaffolding and can be helpful when debugging
int main () {
    int num;
    char binary[MAX_BINARY_STRING_LENGTH];
    char hexadecimal[MAX_HEXADECIMAL_STRING_LENGTH] ;
    num = Get_Number_From_User();
    
    if (num > 0) { 
        Convert_Number_To_Binary(num, binary);
        Convert_Number_To_Hexadecimal(num, binary, hexadecimal);
        printf("num = %d\tbinary = %s\thexadecimal = 0x%s\n",num, binary, hexadecimal);
    }
    return 0;
}

int Get_Number_From_User() {
    // replace the code below
    // use scanf to get a number from the user
    // you may assume that all input is valid
    int num;
	
    printf("Enter a positive integer: ");
    scanf("%d",&num);
    return num;
}

void Convert_Number_To_Binary(const int num,char *binary) {

	/* Code written by myself */
	//https://piazza.com/class/kbfn2knegpi6k8?cid=19
		
	//Check if num is divisible by 2^ith power by shifting the bits of n
	for(int i = 0; i < 32; i++) {
		int shift = num >> i,
		    pos   = 31-i;	//Place the 1/0 in the next least sig digit
		
		//if shift is greater than 0, then num is divisible by 2^i
		if( (shift & 1) > 0 )           
			binary[pos] = '1';
		else
			binary[pos] = '0';
	}
	binary[32] = '\0';		//Null Terminator

}

void Convert_Number_To_Hexadecimal(const int num, const char *binary, char *hexadecimal) {
    // replace the code below
    // write this function on you own without copying from the internet 
    char temp[4];
    int hexPos = 0; //keep track of the next index in hex to change
    
    //Break binary into 8 4-bits sections, and get the hex for each part
    for(int i = 0; i < 29 && hexPos < 9; i+=4) {
    	//printf("i: %d\n", i);
	temp[3] = binary[i];
	temp[2] = binary[i+1];    
    	temp[1] = binary[i+2];
    	temp[0] = binary[i+3];

	getHexChar(temp, &hexadecimal[hexPos]);	
	hexPos++; 
	//printf("tot: %d, Hex: %c\n", tot, hexadecimal[hexPos]);
    }

}

void getHexChar(const char *bin, char *const hexChar) {
        char nums[10] = {'0','1','2','3','4','5','6','7','8','9'};	
	//printf("Bin: %s,", bin);
	int tot = 0;

	for(int i = 0; i < 4; i++) {
		if(bin[i] == 49)
			tot +=  power(2, i);
	}
	
	if(tot == 10)      *hexChar = 'A';
	else if(tot == 11) *hexChar = 'B';
	else if(tot == 12) *hexChar = 'C';
	else if(tot == 13) *hexChar = 'D';
	else if(tot == 14) *hexChar = 'E';
	else if(tot == 15) *hexChar = 'F';
	else		   *hexChar = tot+48;
        

	//printf(", tot: %d, hex: %c\n", tot, *hexChar);

	//return tot;
}

/* Recursively determine base^(pow) */
int power(int base, int pow) {
	return ( (pow > 0) ? base * power(base, pow-1) : 1 );
}


