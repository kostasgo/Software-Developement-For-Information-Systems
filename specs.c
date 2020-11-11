#include "specs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

KV_Pair* createKV(char* key, char* value){
  KV_Pair* data = (KV_Pair*) malloc(sizeof(KV_Pair));
  data->key=strdup(key);
  data->value=strdup(value);
  return data;
}

void deleteKV(KV_Pair* data){
  free(data->key);
  free(data->value);
  free(data);
}

Specs* createSpecsList(char* id){
  Specs* list =(Specs*)malloc(sizeof(Specs));
  list->head=NULL;
  list->size=0;
  list->id=strdup(id);
}

char* createSpecsID(char* directory, char* file){

  char* temp = strdup(file);
  temp[strlen(temp)-5]='\0';

  char s[3]="//";

  char* spec_id = (char*)malloc(strlen(directory) + strlen(temp) + 3);
  bzero(spec_id, strlen(directory)+strlen(temp)+3);

  memcpy(spec_id,directory,strlen(directory));
  memcpy(spec_id + strlen(directory), s, 2);
  memcpy(spec_id + strlen(directory) + 2, temp, strlen(temp));

  free(temp);

  return spec_id;
}

void insertSpecs(Specs** list, char* key, char* value){
  //create KV
  KV_Pair* data=createKV(key, value);
  //create the node
  SpecsNode* node= (SpecsNode*)malloc(sizeof(SpecsNode));

  node->data=data;
  node->next=NULL;

  //if the list is empty, place the node in head
  if((*list)->head==NULL) {
    (*list)->head = node;
  }
  //traverse the list for empty node
  SpecsNode *temp=(*list)->head;
  while(temp->next!= NULL){
    temp=temp->next;
  }

  temp->next=node;
  ((*list)->size)++;
}
