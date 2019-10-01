#include "main.h"

const short HEADER_LEN = 2;		//2 bytes for the key
const short KEY_HEADER = 6;		// 2 bytes for the key, 2 for the pointer, 2 for the length

long delOffset = 0;

struct runs {
	int longPtr;
	int longRun;
};

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

	printBuffer(fileLen, buffer);
	int key = createKey(buffer, fileLen);
	printf("Key: %06X\n\n", key);

	char *compBuffer = compressFile(buffer, fileLen, key);
	printBuffer(fileLen - delOffset, compBuffer);

	if (argc > 2){
		writeToFile(dest, compBuffer, fileLen - delOffset);
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
	char key1 = key & 0xFF;
	char key2 = key >> 8;
	int delOffset = 0;

	tempBuffer[0] = key1;
	tempBuffer[1] = key2;

	for (int i = 0; i < fileLen; i++){
		Struct run = getRun(i, buffer);

		if (run.longRun > KEY_HEADER){
			short tempIndex = i - delOffset + HEADER_LEN;
			tempBuffer[tempIndex] = key1;
			tempBuffer[tempIndex + 1] = key2;
			tempBuffer[tempIndex + 2] = run.longPtr & 0xFF;
			tempBuffer[tempIndex + 3] = run.longPtr >> 8;
			tempBuffer[tempIndex + 4] = run.longRun & 0xFF;
			tempBuffer[tempIndex + 5] = run.longRun >> 8;

			i += run.longRun - 1;
			delOffset += run.longRun - KEY_HEADER;
			printf("i: %i\n", i);
		} else {
			tempBuffer[i - delOffset + HEADER_LEN] = buffer[i];
		}
	}
	return tempBuffer;
}

Struct getRun(int i, char *buffer){
	Struct runs;
	int j = i + SHRT_MIN < 0 ? 0 : i + SHRT_MIN;

	for (; j < i; j++){
		if (buffer[i] == buffer[j]){
			int run = 1;
			while(buffer[i + run] == buffer[j + run]){
				run++;
			}
			if (run > runs.longRun){
				runs.longRun = run;
				runs.longPtr = j;
			}
		}
	}
	return runs;
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
