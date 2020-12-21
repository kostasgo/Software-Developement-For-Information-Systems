#ifndef create_output_h
#define create_output_h

#include "hashtable.h"

void mergeCliques(Hashtable*, char*, char*);
void updateNegatives(Hashtable*, char*, char*);
void parseCsv(char*, Hashtable*);
void outputToFile(Hashtable*);

#endif
