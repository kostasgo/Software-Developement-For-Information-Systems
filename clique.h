#ifndef clique_h
#define clique_h

#include "specs.h"

typedef struct CliqueNodeType{
  /*
  This is a struct for a single node in the clique, which includes
  the specs of one .json file and a pointer to another node.
  */
  Specs* specs;
  struct CliqueNodeType* next;
}Clique;

typedef struct CliqueType{

/*
 This is the struct for the Clique. It consists
 of the list of specs and a size, which will be used
 in the hashtable insert and search functions
*/
  int size;
  CliqueNode* list;
}Clique;

Clique* createClique();
void insertClique(Clique**, Specs*);
void deleteCliqueList(CliqueNode *);


#endif
