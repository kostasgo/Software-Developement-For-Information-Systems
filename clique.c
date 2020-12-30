#include "clique.h"
#include "specs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



Clique* createClique(){
  /*
  Creates an empty Clique structure and returns a pointer to it.
  A Clique struct created by this function should always be deleted
  by deleteClique().
  */

  Clique* clique =(Clique*)malloc(sizeof(Clique));
  clique->list=NULL;
  clique->size=0;
  clique->negatives=NULL;


  return clique;
}


void insertClique(Clique** clique, Specs* specs){
  /*
  Inserts a Specs struct into the given Clique struct.
  */


  //create the node
  CliqueNode* node= (CliqueNode*)malloc(sizeof(CliqueNode));

  node->specs=specs;
  node->next=NULL;
  ((*clique)->size)++;

  //if the list is empty, place the node in list
  if((*clique)->list==NULL) {
    (*clique)->list = node;
    //printf("inserted!\n");
    return;
  }
  //traverse the list for empty node
  CliqueNode *temp=(*clique)->list;
  while(temp->next!= NULL){
    temp=temp->next;
  }

  temp->next=node;
  //printf("inserted!\n");

}

void deleteCliqueList(CliqueNode *list){
  /*
  A recursiv function to delete the list element of Clique. It frees
  the nodes. Does not delete the specs inside them, as they are malloc'd
  elsewhere
  */
  CliqueNode *temp=list;
  CliqueNode* next;

  while(list != NULL){
    next = list->next;
    free(list);
    list=next;
  }
}

void concatCliqueList(CliqueNode* a, CliqueNode* b){
  /*
  Concatenates two Clique lists by attaching the second one
  to the end of the first one.
  */
  //printf("recursion!\n");
  if(a!=NULL && b!=NULL){
    //if we found the end of a, attach b to it
    if(a->next == NULL)
      a->next =b;
    else
    //try for a->next
      concatCliqueList(a->next,b);
  }
  else{
    printf("One of the Cliques is empty\n");
  }
}

/*void mergeCliques(Clique* a, Clique* b){

  Merges two Cliques into one. It concatenates their lists,
  then adjusts the size. In the end it resets the second clique to
  be empty.

  Clique* temp =b;
  concatCliqueList(a->list,temp->list);
  a->size=temp->size + a->size;
  b->size=0;
  b->list=NULL;
}*/


void deleteClique(Clique* clique){
  /*
  Deletes a Clique struct, doing the apropriate frees.
  */
  if(clique!=NULL){
    if(clique->negatives!=NULL){
      deleteNegatives(clique->negatives);
    }

    deleteCliqueList(clique->list);
    free(clique);
  }



}

void printClique(Clique* clique){
  /*
  Prints all the spec_ids inside the clique.
  */

  CliqueNode *temp = clique->list;
  printf("Clique size= %d\n",clique->size);
  for(int i=0; i<clique->size;i++){
    printf("Spec %d: %s\n",i,temp->specs->id);
    temp=temp->next;
  }
}

int isInClique(char* id, Clique* clique){
  CliqueNode* temp = clique->list;
  while(temp!=NULL){
    if(!strcmp(temp->specs->id,id)){
      return 1;
    }
    temp=temp->next;
  }

  return 0;
}

int compareCliques(Clique* cl1, Clique* cl2){
  /*
  Compares two cliques. Returns 1 if they are the same, 0 if not.
  */
  if(!strcmp(cl1->list->specs->id,cl2->list->specs->id)){// they re the same iff their first specs should have the same id
    return 1;
  }
  return 0;
}

int areNegatives(Clique* cl1, Clique* cl2){
  /*
  Looks if the two cliques are negative to each other
  */
  NegListNode* temp=cl1->negatives;

  while(temp!=NULL){
    if(compareCliques(cl2,temp->clique)){
      return 1;
    }
    temp=temp->next;
  }
  return 0;
}
