#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void compressFile(char*, char*);
void decompressFile(char*, char*);

FILE *getFilePtr(char*);
long getSrcLen(FILE*);
long getTargetLen(char[], long);

void createBinaryArray(FILE*, long, int binaryArray[]);
void createHexArray(FILE*, long, char[]);
void writeToFile(char*, int arr[], int);

void charToBinary(int arr[], char);
int compressArray(int binaryArray[], int newArray[], int);

void printBinArray(int arr[], int);
void printCompArray(int arr[], int);



int main (int argc, char **argv) {
	char *option = argv[1];
	char *target = argv[2];
	char *source = argv[3];

	if (argc != 4){
		printf("\nIncorrect format.\n\n");
		printf("To compress:\n");
		printf("./jzip -c target source\n\n");
		printf("To decompress:\n");
		printf("./jzip -d target source\n\n");
		return -1;
	} else if (strcmp(option, "-c") == 0){
		compressFile(target, source);
	} else if (strcmp(option, "-d") == 0){
		decompressFile(target, source);
	} else {
		printf("Use option -c to compress or -d to decompress\n");
		return -1;
	}

	return 0;
}

void compressFile(char *target, char *source){
	FILE *filePtr = getFilePtr(source);
	long fileLen = getSrcLen(filePtr);
	int binaryArray[fileLen * 8];

	createBinaryArray(filePtr, fileLen, binaryArray);

	int writeArray[fileLen * 8 * 2];
	int size = compressArray(binaryArray, writeArray, fileLen * 8);

//	printCompArray(writeArray, lastIndex - 1);

	writeToFile(target, writeArray, size);
}

void decompressFile(char *target, char *source){
	FILE *filePtr = getFilePtr(source);
	long srcLen = getSrcLen(filePtr);
	char hexArray[srcLen];

	createHexArray(filePtr, srcLen, hexArray);

	long targetLen = getTargetLen(hexArray, srcLen);
}



FILE *getFilePtr(char *fileName){
	FILE *filePtr;
	filePtr = fopen(fileName, "rb"); 	 // Open the file in binary mode
	
	return filePtr;
}

long getSrcLen(FILE *filePtr){
	long fileLen;

	fseek(filePtr, 0, SEEK_END);          // Jump to the end of the file
	fileLen = ftell(filePtr);          // Get the current byte offset in the file
	rewind(filePtr);                      // Jump back to the beginning of the file

	return fileLen;
}

long getTargetLen(char arr[], long srcLen){
	long count = 0;
	int i = 0;
	for (; i < srcLen; i=i+2){
		count += arr[i];
	}
	return count;
}

void createBinaryArray(FILE *filePtr, long fileLen, int binaryArray[]){
	char *buffer;
	buffer = (char *) malloc((fileLen + 1) *sizeof(char)); 

	fread(buffer, fileLen, 1, filePtr); // Read in the entire file

	int i = 0;
	for (; i < fileLen; i++){
		int tempArray[8];
		charToBinary(tempArray, buffer[i]);

		int j = 0;
		for(; j < 8; j++){
			binaryArray[i * 8 + j] = tempArray[j];
		}
	}

//	printBinArray(binaryArray, fileLen * 8);

	free(buffer);
	fclose(filePtr); 
}

void createHexArray(FILE *filePtr, long srcLen, char hexArray[]){

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

