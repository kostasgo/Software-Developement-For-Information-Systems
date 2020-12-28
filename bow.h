#ifndef bow_h
#define bow_h

#include "words.h"
#include "linkedlist.h"

typedef struct VocabularyType{
  int max;
  VocBucket** array;
  int total;
}Vocabulary;


void toLower(char*);
char** createStopWordsTable();
void deleteStopWordsTable(char**);
int isStopword(char*, char**);
void filterSpec(Specs*, char**);
Vocabulary* createVocabulary(int, int);
void insertVocabulary(Vocabulary **, char *, double);
int fillVocabulary(Vocabulary **, ListNode*);
void updateScores(Vocabulary*, int);
Word** shrinkTable(Vocabulary*, double, int*);
void deleteVocabulary(Vocabulary*);
Word* searchVocabulary(Vocabulary*, char*);
void printVocabulary(Vocabulary*);

#endif
