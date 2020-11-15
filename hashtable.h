#ifndef hashtable_h
#define hashtable_h
#include "clique.h"

typedef struct HashtableType{
  int max;
  Clique** array;
}Hashtable;


Hashtable* createHashtable(int, int);
unsigned long hash(unsigned char *);
int compress(unsigned long, int);
void insertHashtable(Hashtable**, Specs*, char*);
void deleteHashtable(Hashtable*);


#endif
