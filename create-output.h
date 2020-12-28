#ifndef create_output_h
#define create_output_h

#include "hashtable.h"
#include "bow.h"


void mergeCliques(Hashtable*, char*, char*);
void updateNegatives(Hashtable*, char*, char*);
double* getTfIdfArray(Hashtable*, char*, Vocabulary*, int);
void parseCsv(char*, Hashtable*, Vocabulary*, Word**,int);
void outputToFile(Hashtable*);

#endif
