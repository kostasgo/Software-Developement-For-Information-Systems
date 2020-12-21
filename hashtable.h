#ifndef hashtable_h
#define hashtable_h
#include "bucket.h"

typedef struct HashtableType{
  int max;
  Bucket** array;
}Hashtable;


Hashtable* createHashtable(int, int);
unsigned long hash(unsigned char *);
int compress(unsigned long, int);
void insertHashtable(Hashtable**, Specs*);
void deleteHashtable(Hashtable*);
BucketData* searchHashtable(Hashtable*, char*);

void printHashtable(Hashtable*);


#endif
