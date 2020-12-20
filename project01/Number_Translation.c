#include <stdio.h>

// avoid hardcoding with numeric constants for array lengths
#define MAX_BINARY_STRING_LENGTH 33
#define MAX_HEXADECIMAL_STRING_LENGTH 9

// function declarations (prototypes)
// do not change these
int Get_Number_From_User();
void Convert_Number_To_Binary(const int num,char *binary);
void Convert_Number_To_Hexadecimal(const int num, const char *binary, char *hexadecimal);
void getHexChar(const char *const bin, char *const hexadecimal);
int  power(int base, int pow);

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
    int num; //store user num
    printf("Enter a positive integer: ");
    scanf("%d",&num);
    return num;
}

void Convert_Number_To_Binary(const int num,char *binary) {

	/* Code written by myself */
	//https://piazza.com/class/kbfn2knegpi6k8?cid=19
	
	int firstI = -1;
	char temp[MAX_BINARY_STRING_LENGTH];

	//Check if num is divisible by 2^ith power by shifting the bits of n
	for(int i = 0; i < 32; i++) {
		int shift = num >> i,
		    pos   = 31-i;	//Place the 1/0 in the next least sig digit
		
		//if shift is greater than 0, then num is divisible by 2^i
		if( (shift & 1) > 0 ) {          
			temp[pos] = '1';
			firstI = pos;
		}
		else
			temp[pos] = '0';
	}

	//Get rid of leading zeros, if any exist, and copy
	//sig figs from temp into bin
	int i = 0;
	for(; firstI < 32; firstI++) {
		binary[i] = temp[firstI];
		i++;
	}

	binary[i] = '\0'; // Null terminator
}

void Convert_Number_To_Hexadecimal(const int num, const char *binary, char *hexadecimal) {
    char temp[5];
    int nullTerm = 0, hexPos = 0;
  
    //Find how long the binary string is (\0 comes before 32)
    for(int i = 0; i < 32; i++) {
        if(binary[i] == '\0') {
            nullTerm = i;
            break;
        }
    }

    //Break bin into approrpiate 4-bit sections and convert those into the correct hex equivelant
    for(int i = nullTerm-1; i > -1; i -= 4) {
        
	//Construct the bit sequence from the back of bin (ex: 1001011, the first sequence is 1011)
	temp[0] = ( binary[i-3] == 48 || binary[i-3] == 49) ? binary[i-3] : '0';  //binary[i-3,2,1] doesnt exist fill it with 0
        temp[1] = ( binary[i-2] == 48 || binary[i-2] == 49) ? binary[i-2] : '0';
        temp[2] = ( binary[i-1] == 48 || binary[i-1] == 49) ? binary[i-1] : '0';
        temp[3] = ( binary[i]   == 48 || binary[i]   == 49) ? binary[i]   : '0';
        getHexChar(temp, &hexadecimal[hexPos]);  				  //convert the 4-bit section into hex
        hexPos++;
    }
    
    // Find the last character and terminate the hex char array
    for(int i = 0; i < 32; i++) {
	    //48-90 is the range from 0 to A on ascii
	    if(hexadecimal[i] < 48 || hexadecimal[i] > 90) {
		    hexadecimal[i] = '\0';
		    break;
	    }
    }
    
    //Reverse hexadecimal array
    for(int i=0; i < hexPos/2; i++) {
	    char t = hexadecimal[i];
	    hexadecimal[i] = hexadecimal[hexPos-1-i];
	    hexadecimal[hexPos-i-1] = t;
    }
}

//Helper method: convert 4-bit binary sequence into hex
void getHexChar(const char *const bin, char *const hexadecimal) {
    int tot = 0;
    
    //Determine the decimal equivalent of this sequence
    for(int i = 0; i < 4; i++) {
        if(bin[i] == 49) tot += power(2, (3-i) );
    }
    
    //convert the decimal portion into hex
    if      (tot == 10)  *hexadecimal = 'A';
    else if (tot == 11)  *hexadecimal = 'B';
    else if (tot == 12)  *hexadecimal = 'C';
    else if (tot == 13)  *hexadecimal = 'D';
    else if (tot == 14)  *hexadecimal = 'E';
    else if (tot == 15)  *hexadecimal = 'F';
    else if (tot <  10)  *hexadecimal = tot+48; //add 48 for the approrpiate ascii value
}

/* Recursively determine base^(pow) */
int power(int base, int pow) {
	return ( (pow > 0) ? base * power(base, pow-1) : 1 );
}
