#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyvalue.h"

void insertValue(Value** value, char* str){
  /*
  inserts a new string to the value struct.
  */
  Value* node = (Value*)malloc(sizeof(Value));
  node->next=NULL;
  //printf(str);
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
  Deletes a value struct.
  */

  Value* next;
  while(value != NULL){


    next = value->next;
    free(value->str);
    free(value);
    value=next;
  }

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
  //printf("%s\n",key);
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
  deleteValue(data->value);
  free(data->key);

  free(data);
}
