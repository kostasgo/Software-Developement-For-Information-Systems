#ifndef hashtable_h
#define hashtable_h
#include "clique.h"

typedef struct HashtableType{
  int max;
  Clique** array;
}Hashtable;


Hashtable* createHashtable(int);
unsigned long hash(unsigned char *);
int compress(unsigned long, int);
void insertHashtable(Hashtable**, Specs*);
void deleteHashtable(Hashtable*);
int searchHashtable(Hashtable*, char*);
void printHashtable(Hashtable*);


#endif
