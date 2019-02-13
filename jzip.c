#include <stdio.h>
#include <stdlib.h>


FILE *getFilePtr(char*);
long getFileLength(FILE*);
void createBinaryArray(FILE*, long, int binaryArray[]);

void charToBinary(int arr[], char);
void binaryToChar(int arr[], int, int);
int compressArray(int binaryArray[], int newArray[], int arrLen);
void printBinArray(int arr[], int);


FILE *writeptr;
char *newBuffer;

int main (int argc, char **argv) {
	FILE *filePtr = getFilePtr(argv[1]);
	long fileLength = getFileLength(filePtr);
	int binaryArray[fileLength * 8];

	createBinaryArray(filePtr, fileLength, binaryArray);






	int newArray[fileLength * 8 * 2];
	int lastIndex = compressArray(binaryArray, newArray, fileLength * 8);

//	printBinArray(newArray, lastIndex - 1);

	newBuffer = (char *)malloc((lastIndex/4 + 1) * sizeof(char));

	writeptr = fopen("new.out", "wb");
	fwrite(newBuffer, sizeof(newBuffer), 1, writeptr);

	fclose(writeptr);
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

	printBinArray(binaryArray, fileLength * 8);
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

void binaryToChar(int arr[], int count, int bit){

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
