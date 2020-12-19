#include "linkedlist.h"
#include "specs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void insertList(ListNode** L, Specs *s){
  ListNode *n;
  //printf("Another insert\n");

  n = (ListNode *)malloc(sizeof(ListNode));
  n->specs = s;
  n->next = NULL;
  if((*L)==NULL) {
    //printf("Inserting specs %d at first node\n",p->id);
    *L = n;
    return;
  }
  ListNode *temp=*L;
  while(temp->next!= NULL){
    temp=temp->next;
  }

  temp->next=n;

}

int list_length(ListNode* L){
  int r=0;
  while(L){
    r++;
  }
  return r;
}

void printList(ListNode*L){

  while(L!=NULL){
    printSpecs(L->specs);
    L=L->next;
  }
  printf("NULL\n");
}

void deleteList(ListNode *L){

  ListNode *temp=L;
  ListNode* next;
/*
  while(L!= NULL){
    next = L-> next;
    deletespecs(L->specs);
    free(L);
    L = next;
  }
*/
  if (L==NULL)
    return;
  deleteList(L->next);
  deleteSpecs(L->specs);
  free(L);
  //*L = NULL;

}

Specs * searchList(ListNode *L, char* id){
  ListNode *temp=L;
  while(temp!=NULL){
    if(!strcmp(temp->specs->id,id))
      return temp->specs;
    temp = temp->next;
  }
  return NULL;
}
