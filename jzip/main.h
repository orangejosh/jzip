#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

typedef struct runs Struct;

long getSrcLen(FILE *filePtr);
char *createBuffer(char *source, FILE *filePtr, long fileLen);
int createKey(char *buffer, long fileLen);
bool compareBytes(short key, char charByte, int byteIndex);
char *compressFile(char *buffer, long fileLen, short key);
Struct getRun(int i, char *buffer);
void writeToFile(char *fileName, char *buffer, int size);
void printBuffer(long fileLen, char *buffer);
