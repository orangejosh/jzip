#include <stdio.h>
#include <stdlib.h>

void charToBinary(int arr[], char);
int compressArray(int binaryArray[], int newArray[], int arrLen);
void printBinArray(int arr[], int);

FILE *fileptr;
char *buffer;
long filelen;

FILE *writeptr;
char *newBuffer;

int main (int argc, char **argv) {
	fileptr = fopen(argv[1], "rb"); 	 // Open the file in binary mode
	fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
	filelen = ftell(fileptr);             // Get the current byte offset in the file
	rewind(fileptr);                      // Jump back to the beginning of the file
	buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
	fread(buffer, filelen, 1, fileptr); // Read in the entire file

	int binaryArray[filelen * 8];

	int i = 0;
	for (; i < filelen; i++){
		int tempArray[8];
		charToBinary(tempArray, buffer[i]);

		int j = 0;
		for(; j < 8; j++){
			binaryArray[i * 8 + j] = tempArray[j];
		}
	}

//	printBinArray(binaryArray, filelen * 8);

	int newArray[filelen * 8 * 2];
	int lastIndex = compressArray(binaryArray, newArray, filelen * 8);

//	printBinArray(newArray, lastIndex - 1);

	newBuffer = (char *)malloc((lastIndex/4 + 1) * sizeof(char));

	writeptr = fopen("new.out", "wb");
	fwrite(newBuffer, sizeof(newBuffer), 1, writeptr);

	fclose(fileptr); // Close the file
	fclose(writeptr);
}


void charToBinary (int arr[], char ch){
	unsigned int num = (unsigned int) ch;

	int i = 0;
	for (; i < 8; i++){
		arr[7 - i] = num & 0x1;
		num = num >> 1;
	}
}


int compressArray(int binaryArray[], int newArray[], int arrLen){
	int lastBit = binaryArray[0];
	int bitCount = 1;
	int newIndex = 0;

	int i = 1;
	for (; i < arrLen; i++){
		if (lastBit != binaryArray[i]){
			newArray[newIndex] = bitCount;
			newArray[newIndex + 1] = lastBit;
			newIndex = newIndex + 2;
			lastBit = binaryArray[i];
			bitCount = 1;
		} else if (i == arrLen - 1){
			newArray[newIndex] = bitCount;
			newArray[newIndex + 1] = lastBit;
		} else {
			bitCount++;
		}
	}
	return newIndex + 1;
}

void printBinArray(int arr[], int arrLen){
	int i = 0;
	for(; i < arrLen; i++){
		if (i % 8 == 0 && i != 0) printf(" ");
		if (i % 48 == 0 && i != 0) printf("\n");
		printf("%d", arr[i]);
	}

	printf("\n");
}

