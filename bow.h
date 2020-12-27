#ifndef vocabulary_h
#define vocabulary_h

#include "words.h"
#include "linkedlist.h"

typedef struct BoWType{
  int max;
  VocBucket** array;
  int total;
}BoW;

void toLower(char*);
char** createStopWordsTable();
void deleteStopWordsTable(char**);
int isStopword(char*, char**);
void filterSpec(Specs*, char**);
BoW* createBoW(int, int);
void insertBoW(BoW **, char *);
void fillVocabulary(BoW **, ListNode*);
void deleteBoW(BoW*);
Word* searchBoW(BoW*, char*);
void printBoW(BoW*);

#endif
