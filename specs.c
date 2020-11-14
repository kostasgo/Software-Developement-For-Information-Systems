#include "specs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

KV_Pair* createKV(char* key, char* value){
  /*
  Creates a KV_Pair struct and returns a pointer to it. Takes as input two strings,
  a key and a value. A KV_Pair struct that is created by this function should always be
  destroyed by deleteKV().
  */

  KV_Pair* data = (KV_Pair*) malloc(sizeof(KV_Pair));
  data->key=strdup(key);
  data->value=strdup(value);
  return data;
}

void deleteKV(KV_Pair* data){
  /*
  Deletes a KV_Pair struct, doing the apropriate frees.
  */
  free(data->key);
  free(data->value);
  free(data);
}

char* createSpecsID(char* directory, char* file){
  /*
  Creates a string spec_id in the format that is requested by the csv files.
  For example, if the given directory is "www.ebay.com" and the file is "43002.json",
  the result id wil be "www.ebay.com//43002". A string created by this function should
  always be deleted by deleteSpecsID().
  */

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

void deleteSpecsId(char* spec_id){
  /*
  Deletes a spec_id string by freeing it. This function may seem redundant,
  but we want to avoid the use of functions like malloc() and free() in the
  main programm.
  */

  free(spec_id);
}

Specs* createSpecs(char* id){
  /*
  Creates an empty Specs structure and returns a pointer to it.
  Takes as argument a string, the specs_id. A Specs struct created
  by this function should always be deleted by deleteSpecs().
  */

  Specs* specs =(Specs*)malloc(sizeof(Specs));
  specs->list=NULL;
  specs->size=0;
  specs->id=strdup(id);

  return specs;
}


void insertSpecs(Specs** specs, char* key, char* value){
  /*
  Inserts a key-value, pair into the given Specs struct. The pair structure
  is created inside the function, to limit the numer of functions that need
  to be called by the main programm.
  */

  //create KV
  KV_Pair* data=createKV(key, value);
  //create the node
  SpecsNode* node= (SpecsNode*)malloc(sizeof(SpecsNode));

  node->data=data;
  node->next=NULL;

  //if the list is empty, place the node in list
  if((*specs)->list==NULL) {
    (*specs)->list = node;
  }
  //traverse the list for empty node
  SpecsNode *temp=(*specs)->list;
  while(temp->next!= NULL){
    temp=temp->next;
  }

  temp->next=node;
  ((*specs)->size)++;
}

void deleteSpecsList(SpecsNode *list){
  /*
  A recursiv function to delete the list element of Specs. It frees
  the nodes and also deletes the key-value pairs inside them.
  */
  SpecsNode *temp=list;
  SpecsNode* next;

  if (list==NULL)
    return;
  deleteSpecsList(list->next);
  deleteKV(list->data);
  free(list);
  //*L = NULL;

}
/*
Deletes a Specs struct, doing the apropriate frees.
*/
void deleteSpecs(Specs* specs){
  deleteSpecsList(specs->list);
  free(specs->id);
  free(specs);
}
