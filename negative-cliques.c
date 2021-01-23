#include "negative-cliques.h"
#include "clique.h"
#include <stdlib.h>

void insertNegatives(NegListNode** list, Clique* clique){
  /*
  Inserts a node into a list of Cliques.
  */
  NegListNode *n = (NegListNode *)malloc(sizeof(NegListNode));
  n->clique = clique;
  n->next = NULL;
  n->dirtyBit = 0;
  if((*list)==NULL) {
    //printf("Inserting specs %d at first node\n",p->id);
    *list = n;
    return;
  }
  NegListNode *temp=*list;
  while(temp->next!= NULL){
    if(compareCliques(temp->clique,clique)){ //if clique is already noted as a negative we just stop
      return;
    }
    temp=temp->next;
  }

  temp->next=n;

}

void deleteNegatives(NegListNode* list){
  /*
  Deletes the space allocated for the nodes of the list of
  negative cliques(without deleting the respective cliques)
  */

  NegListNode *temp=list;
  NegListNode* next;

  while(list != NULL){
    next = list->next;
    free(list);
    list=next;
  }

}
