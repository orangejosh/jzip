#include "main.h"

const short headerLen = 2;		//2 bytes for the key
const short keyHeader = 6;		// 2 bytes for the key, 2 for the pointer, 2 for the length

long lenOffset = 0;

int main (int argc, char **argv) {
	char *src = argv[1];
	char *dest;

	if (argc > 2){
		dest = argv[2];
	}

	FILE *filePtr = fopen(src, "rb");
	long fileLen = getSrcLen(filePtr);
	char *buffer = createBuffer(src, filePtr, fileLen);

	fclose(filePtr); 

//	printBuffer(fileLen, buffer);
	int key = createKey(buffer, fileLen);
	printf("Create Key\n");

	char *compBuffer = compressFile(buffer, fileLen, key);
	printf("Compress\n");
//	printBuffer(fileLen - lenOffset, compBuffer);

	if (argc > 2){
		writeToFile(dest, compBuffer, fileLen - lenOffset);
	}

	free(buffer);
	free(compBuffer);
}


long getSrcLen(FILE *filePtr){
	fseek(filePtr, 0, SEEK_END);
	long fileLen = ftell(filePtr);
	rewind(filePtr);

	return fileLen;
}

char *createBuffer(char *source, FILE *filePtr, long fileLen){
	char *buffer = (char *) malloc((fileLen + 1) * sizeof(char)); 
	fread(buffer, fileLen, 1, filePtr);

	return buffer;
}

int createKey(char *buffer, long fileLen){
	bool unique;
	unsigned short key;
	srand(time(0));

	do {
		key = rand() % 65535;
		unique = true;

		for (int i = 0; i < fileLen - 1; i++){
			if ((key & 0xFF) == buffer[i] && (key >> 8) == buffer[i + 1]){
				unique = false;
				break;
			}
		}
	} while (!unique);

	return key;
}

char *compressFile(char *buffer, long fileLen, short key){
	char *tempBuffer = (char *) malloc((fileLen + 1) * sizeof(char)); 
	unsigned char key1 = key & 0xFF;
	unsigned char key2 = key >> 8;

	tempBuffer[0] = key1;
	tempBuffer[1] = key2;

	for (int i = 0; i < fileLen; i++){
		int j = i + SHRT_MIN < 0 ? 0 : i + SHRT_MIN;
		if (i == j){
			tempBuffer[i + headerLen] = buffer[j];
		}

		for (; j < i; j++){
			short offset = 0;
			while(buffer[i + offset] == buffer[j + offset]){
				offset++;
			}

			short tempIndex = i - lenOffset + headerLen;
			if (offset <= keyHeader){
				tempBuffer[tempIndex] = buffer[i];
			} else {
				tempBuffer[tempIndex] = key1;
				tempBuffer[tempIndex + 1] = key2;
				tempBuffer[tempIndex + 2] = (tempIndex - j) & 0xFF;
				tempBuffer[tempIndex + 3] = (tempIndex - j) >> 8;
				tempBuffer[tempIndex + 4] = offset & 0xFF;
				tempBuffer[tempIndex + 5] = offset >> 8;

				i += offset;
				lenOffset += offset - keyHeader;
			}
		}
	}
	return tempBuffer;
}

void writeToFile(char *fileName, char *buffer, int size){
	FILE *writePtr = fopen(fileName, "wb");
	char *writeBuffer = (char *) malloc((size + 1) * sizeof(char));
	int i = 0;
	for (; i < size; i++){
		writeBuffer[i] = buffer[i];
	}

	fwrite(writeBuffer, sizeof(char), i - 1, writePtr);

	free(writeBuffer);
	fclose(writePtr);
}

void printBuffer(long fileLen, char *buffer){
	for (int i = 0; i < fileLen; i++){
		printf("%#06X \n", buffer[i]);
	}

}
