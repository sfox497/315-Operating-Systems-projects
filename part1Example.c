#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int * ToBinary(int n);

int main(){
	int integer,LSB, MSB;
	int *final;

	FILE *file;
    int lines = 0;
    char character;

	//opening file
    file = fopen("part1Input.txt", "r");
	//if file doesn't exist or can't open
	if (file == NULL){
        printf("Could not open file part1Input.txt\n");
        return 1;
    }
   	//extract characters from file
    character = getc(file);
	//while the character is not the end of the file
    while (character != EOF){
        //Count whenever there is a new line
        if (character == '\n'){
            lines++;
        }
        //get next character
        character = getc(file);
    }
	//count the last line as it does not have \n
	lines++;
	//close file to read from it again
    fclose(file);

	//open again to read again
	file = fopen("part1Input.txt", "r");
	char tempL[10], tempM[10];
	//get LSB
	fgets(tempL, 10, file);
	LSB = atoi(tempL);
	printf("LSB: %d\n", LSB);
	
	//get MSB
	fgets(tempM, 10, file);
	MSB = atoi(tempM);
	printf("MSB: %d\n", MSB);

	//going through the rest of the lines in the file (vitrual address)
	for (int address = 2; address < lines; address++){
		char v[10];
		fgets(v, 10, file);
		integer = atoi(v);

		//getting page number from most significant bits
		int page = integer >> LSB;	
		//getting the offset from the least significant bits
		int offset = integer-(page << LSB);


		printf("%d. Virtual address %d is in page number %d and offset %d\n",(address-1), integer, page, offset);
		//test the results
		printf("In binary to check: Virtual address = ");
		final = ToBinary(integer);
		for (int i = 0; i < 16; i++){
			printf("%d", final[i]);
		}
		printf(", page number = ");
		final = ToBinary(page);
		for (int i = 0; i < 16; i++){
			printf("%d", final[i]);
		}
		printf(", offset number = ");
		final = ToBinary(offset);
		for (int i = 0; i < 16; i++){
			printf("%d", final[i]);
		}
		printf("\n---------\n");
	}

	//close file
    fclose(file);
    return 0; 

}
int * ToBinary(int n){
	int binaryNum[16];
	static int binary[16];  
  
    // counter for binary array 
    int i = 0; 
    while (n > 0) { 
  		// storing remainder in binary array 
        binaryNum[i] = n % 2; 
        n = n / 2; 
        i++; 
    }
	int zeros = 16-i;
	//putting zeros in front if needed
	for (int i = 0; i < zeros; i++){
		binary[i] = 0;
	}
    // putting array in return array the right way and reusing zero
    for (int j = i - 1; j >= 0; j--) {
		binary[zeros] = binaryNum[j];
		zeros++;
	}
	return binary;
}