#ifndef vocabulary_h
#define vocabulary_h

#include "words.h"

typedef struct VocTableType{
  int max;
  VocBucket** array;
}VocTable;

void toLower(char*);
char** createStopWordsTable();
void deleteStopWordsTable(char**);
int isStopword(char*, char**);
void filterSpec(Specs*, char**);

#endif
