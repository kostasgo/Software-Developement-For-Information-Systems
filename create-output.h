#ifndef create_output_h
#define create_output_h

#include "hashtable.h"
#include "bow.h"
#include "logistic_regression.h"

void swapStrings(char**, char**);
void shuffleArray(void* , size_t);
void mergeCliques(Hashtable*, char*, char*);
void updateNegatives(Hashtable*, char*, char*);
double* getTfIdfArray(Hashtable*, char*, Vocabulary*, int);
void parseCsv(char*, Hashtable*, Vocabulary*, int, Classifier*);
void outputToFile(Hashtable*);

#endif
