#ifndef negative_cliques_h
#define negative_cliques_h

#include "clique.h"

typedef struct NegListNodeType{
  struct CliqueType* clique;
  struct NegListNodeType* next;
}NegListNode;

void insertNegatives(NegListNode**, struct CliqueType*);
#endif
