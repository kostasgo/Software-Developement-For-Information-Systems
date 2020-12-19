#ifndef linkedlist_h
#define linkedlist_h

#include "specs.h"

typedef struct LNodeType{
  Specs *specs;
  struct LNodeType *next;
}ListNode;

void insertList(ListNode**, Specs*);
int list_length(ListNode*);
void deleteList(ListNode*);
void printList(ListNode*);
Specs * searchList(ListNode *, char*);

#endif
