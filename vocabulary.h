#ifndef vocabulary_h
#define vocabulary_h

#include "words.h"
#include "linkedlist.h"

typedef struct VocTableType{
  int max;
  VocBucket** array;
  int total;
}VocTable;

void toLower(char*);
char** createStopWordsTable();
void deleteStopWordsTable(char**);
int isStopword(char*, char**);
void filterSpec(Specs*, char**);
VocTable* createVocTable(int, int);
void insertVocTable(VocTable **, char *);
void fillVocabulary(VocTable **, ListNode*);
void deleteVocTable(VocTable*);
Word* searchVocTable(VocTable*, char*);
void printVocTable(VocTable*);

#endif
