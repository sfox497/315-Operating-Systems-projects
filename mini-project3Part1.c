#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	int integer,LSB, MSB;
	int *final;

	FILE *file;
    int lines = 0;
    char filename[10], character;

	//reading from file
    printf("Enter input file name: ");
    scanf("%s", filename);
	//opening file
    file = fopen(filename, "r");
	//if file doesn't exist or can't open
	if (file == NULL){
        printf("Could not open file %s\n",filename);
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
	file = fopen(filename, "r");
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
	}
	//close file
    fclose(file);
    return 0; 
}