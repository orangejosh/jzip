#include <stdio.h>
#include <stdlib.h>


FILE *getFilePtr(char*);
long getFileLength(FILE*);
void createBinaryArray(FILE*, long, int binaryArray[]);
void writeToFile(char*, int arr[], int size);

void charToBinary(int arr[], char);
int compressArray(int binaryArray[], int newArray[], int arrLen);

void printBinArray(int arr[], int);
void printCompArray(int arr[], int);



int main (int argc, char **argv) {
	FILE *filePtr = getFilePtr(argv[2]);
	long fileLength = getFileLength(filePtr);
	int binaryArray[fileLength * 8];

	createBinaryArray(filePtr, fileLength, binaryArray);

	int writeArray[fileLength * 8 * 2];
	int size = compressArray(binaryArray, writeArray, fileLength * 8);

//	printCompArray(writeArray, lastIndex - 1);

	writeToFile(argv[1], writeArray, size);

}

FILE *getFilePtr(char *fileName){
	FILE *filePtr;
	filePtr = fopen(fileName, "rb"); 	 // Open the file in binary mode
	
	return filePtr;
}

long getFileLength(FILE *filePtr){
	long fileLength;

	fseek(filePtr, 0, SEEK_END);          // Jump to the end of the file
	fileLength = ftell(filePtr);          // Get the current byte offset in the file
	rewind(filePtr);                      // Jump back to the beginning of the file

	return fileLength;
}


void createBinaryArray(FILE *filePtr, long fileLength, int binaryArray[]){
	char *buffer;
	buffer = (char *) malloc((fileLength + 1) *sizeof(char)); 

	fread(buffer, fileLength, 1, filePtr); // Read in the entire file

	int i = 0;
	for (; i < fileLength; i++){
		int tempArray[8];
		charToBinary(tempArray, buffer[i]);

		int j = 0;
		for(; j < 8; j++){
			binaryArray[i * 8 + j] = tempArray[j];
		}
	}

//	printBinArray(binaryArray, fileLength * 8);

	free(buffer);
	fclose(filePtr); 
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
		if (lastBit != binaryArray[i] || bitCount >= 255){
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

void writeToFile(char *fileName, int arr[], int size){
	FILE *writePtr = fopen(fileName, "wb");
	unsigned char *buffer = (unsigned char *) malloc((size + 1) *sizeof(unsigned char));
	int i = 0;
	for (; i < size; i++){
		buffer[i] = arr[i];
	}

	fwrite(buffer, sizeof(unsigned char), i - 1, writePtr);

	free(buffer);
	fclose(writePtr);
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

void printCompArray(int arr[], int arrLen){
	int i = 0;
	for(; i < arrLen; i++){
		if (i % 8 == 0 && i != 0) printf(" ");
		if (i % 48 == 0 && i != 0) printf("\n");
		if (i % 2 == 0) {
			printf("%d", arr[i]);
		} else {
			printf("%d ", arr[i]);
		}
	}

	printf("\n");
}

