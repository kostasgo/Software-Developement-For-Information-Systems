#include "negative-cliques.h"
#include "clique.h"
#include <stdlib.h>

void insertNegatives(NegListNode** list, Clique* clique){

  NegListNode *n = (NegListNode *)malloc(sizeof(NegListNode));
  n->clique = clique;
  n->next = NULL;
  if((*list)==NULL) {
    //printf("Inserting specs %d at first node\n",p->id);
    *list = n;
    return;
  }
  NegListNode *temp=*list;
  while(temp->next!= NULL){
    if(compareCliques(temp->clique,clique)){
      return;
    }
    temp=temp->next;
  }

  temp->next=n;

}
