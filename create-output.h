#ifndef create_output_h
#define create_output_h

#include "hashtable.h"
#include "bow.h"
#include "logistic_regression.h"

void swapStrings(char**, char**);
void shuffleArray(void* , size_t);
void mergeCliques(Hashtable*, char*, char*);
void updateNegatives(Hashtable*, char*, char*);
SparseV* getTfIdfArray(Hashtable*, char*, Vocabulary*, int);
SparseV** createX(char**, int, int, Hashtable*, Vocabulary*, int);
int* createY(char**, int, int);
void parseCsv(char*, Hashtable*, Vocabulary*, int, Classifier*);
int validate(char**, int, Classifier*, Hashtable*, Vocabulary*);
void testing(char** , int, Classifier*, Hashtable*, Vocabulary*);

#endif
