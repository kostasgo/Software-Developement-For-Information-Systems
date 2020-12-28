#ifndef specs_h
#define specs_h

#include "keyvalue.h"

typedef struct SpecsNodeType{
  /*
  This structure is a list of key-value pairs,
  and is furtherly being used in the struct Specs
  */
  KV_Pair* data;
  struct SpecsNodeType *next;
} SpecsNode;

typedef struct CorrectNodeType{
  /*
  This is a list of all the words in the file after they have been filtered.
  */
  char* word;
  int counter;
  double tf;
  struct CorrectNodeType* next;
} CorrectNode;

typedef struct SpecsType{
  /*
  This is the Specs structure, where the .json files are
  going to be saved. It consists of the file's id, the list
  of key-value pairs, and an int to keep track of the size, which
  may, or may not, be useful later. We also added a list of all the
  words in the .json file that are not punctuation, stopwords and are
  all in lowercase.
  */
  SpecsNode* list;
  int size;
  char* id;
  CorrectNode* words;
  int total;
} Specs;


char* createSpecsID(char*, char*);
void insertCorrect(Specs **, char*);
void deleteSpecsId(char*);
Specs* createSpecs(char*);
void insertSpecs(Specs**, KV_Pair*);
void printSpecs(Specs*);
void deleteSpecsList(SpecsNode *);
void deleteCorrectList(CorrectNode *);
void deleteSpecs(Specs*);

#endif
