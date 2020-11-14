#include "Clique.h"
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

  //if the list is empty, place the node in list
  if((*clique)->list==NULL) {
    (*clique)->list = node;
  }
  //traverse the list for empty node
  CliqueNode *temp=(*clique)->list;
  while(temp->next!= NULL){
    temp=temp->next;
  }

  temp->next=node;
  ((*clique)->size)++;
}

void deleteCliqueList(CliqueNode *list){
  /*
  A recursiv function to delete the list element of Clique. It frees
  the nodes and also deletes the specs inside them.
  */
  CliqueNode *temp=list;
  CliqueNode* next;

  if (list==NULL)
    return;
  deleteCliqueList(list->next);
  deleteSpecs(list->specs);
  free(list);
}

/*
Deletes a Clique struct, doing the apropriate frees.
*/
void deleteClique(Clique* clique){
  deleteCliqueList(clique->list);
  free(clique);
}
