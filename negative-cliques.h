#ifndef negative_cliques_h
#define negative_cliques_h

#include "clique.h"

typedef struct NegListNodeType{
  /*
  This is a list of cliques. Its purpose is to represent
  the set of cliques that are negatively related to each unique clique.
  That way, when we see a negative relation in the csv file, we just update
  both cliques with each other.
  */
  struct CliqueType* clique;
  struct NegListNodeType* next;
  int dirtyBit; //this byte is used when creating the final set
  
}NegListNode;

void insertNegatives(NegListNode**, struct CliqueType*);
void deleteNegatives(NegListNode*);
#endif
