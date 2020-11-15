#include "specs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void insertValue(Value** value, char* str){
  /*
  inserts a new string to the value struct.
  */
  Value* node = (Value*)malloc(sizeof(Value));
  node->next=NULL;
  node->str=strdup(str);

  Value *temp;

  //if the list is empty, place the node in list
  if(*value==NULL) {
    *value = node;
    return;
  }

  //traverse the list for empty node

	temp=(*value);
	while(temp->next!= NULL){
    temp=temp->next;
	}
	temp->next=node;
}

void deleteValue(Value* value){
  /*
  Deletes a value struct recursively.
  */
  Value *temp=value;
  Value* next;

  if (value==NULL)
    return;
  deleteValue(value->next);
  free(value->str);
  free(value);
}

void printValue(Value* value){
  Value* temp=value;
  if(temp==NULL){
    return;
  }
  printf("[");
  while(temp->next!=NULL){
    printf("%s, ", temp->str);
    temp=temp->next;
  }
  printf("%s]\n", temp->str);
}

KV_Pair* createKV(char* key, Value* value){
  /*
  Creates a KV_Pair struct and returns a pointer to it. Takes as input two strings,
  a key and a value. A KV_Pair struct that is created by this function should always be
  destroyed by deleteKV().
  */

  KV_Pair* data = (KV_Pair*) malloc(sizeof(KV_Pair));
  data->key=strdup(key);
  data->value=value;
  return data;
}


void printKV(KV_Pair* pair){
  /*
  Takes as input a pointer to KV_Pair and prints its key and value.
  */
  printf("Key: %s Value: ", pair->key);\
  printValue(pair->value);

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


void insertSpecs(Specs** specs, KV_Pair* data){
  /*
  Inserts a key-value, pair into the given Specs struct.
  */


  //create the node
  SpecsNode* node= (SpecsNode*)malloc(sizeof(SpecsNode));

  ((*specs)->size)++;

  node->data=data;
  node->next=NULL;
  SpecsNode *temp;

  //if the list is empty, place the node in list
  if((*specs)->list==NULL) {
    (*specs)->list = node;
  }

  //traverse the list for empty node
  else{
	  temp=(*specs)->list;
	  while(temp->next!= NULL){
		temp=temp->next;
	}
		temp->next=node;
  }

}


void printSpecs(Specs* specs){
  /*
  Takes as input a pointer to Specs and prints its id
  and then all the pairs of keys and values it contains calling the printKV() function for each pair.
  */

  SpecsNode *current_node = specs->list;

  printf ("Spec ID: %s\n\nPairs of keys and values for this Spec ID\n", specs->id);

  for(int i = 0; i < specs->size; i++){
	   printKV(current_node->data);
	current_node = current_node->next;
  }

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


}


/*
Deletes a Specs struct, doing the apropriate frees.
*/
void deleteSpecs(Specs* specs){
  deleteSpecsList(specs->list);
  //free(specs->id);
  //free(specs);
}
