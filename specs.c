#include "specs.h"
#include "keyvalue.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
  specs->words=NULL;
  specs->total=0;

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

void insertCorrect(Specs ** specs, char* str){
  /*
  Inserts a word to the list of filtered words in the specwww.ebay.com//24972,www.ebay.com//45946,0

  */
  CorrectNode *n;
  //printf("Another insert\n");

  n = (CorrectNode *)malloc(sizeof(CorrectNode));
  n->word = strdup(str);
  n->counter = 1;
  n->next = NULL;
  n->tf = 0;
  (*specs)->total++;

  if((*specs)->words==NULL) {
    //printf("Inserting specs %d at first node\n",p->id);
    (*specs)->words = n;
    return;
  }
  CorrectNode *temp=(*specs)->words;

  while(temp->next!= NULL){
    if(!strcmp(temp->word,str)){
      free(n);
      temp->counter++;
      return;
    }
    temp=temp->next;
  }

  temp->next=n;
}



void printSpecs(Specs* specs){
  /*
  Takes as input a pointer to Specs and prints its id
  and then all the pairs of keys and values it contains calling the printKV() function for each pair.
  */

  CorrectNode *current_node = specs->words;

  printf ("SPEC ID: %s\n\nTOTAL WORDS: %d\n\n", specs->id, specs->total);

  while(current_node!=NULL){
    printf("WORD: %s||COUNTER: %d\n",current_node->word, current_node->counter);
    current_node = current_node->next;
  }

}


void deleteSpecsList(SpecsNode *list){
  /*
  A function to delete the list element of Specs. It frees
  the nodes and also deletes the key-value pairs inside them.
  */
  SpecsNode* next;
  while(list != NULL){

    deleteKV(list->data);
    next = list->next;
    free(list);
    list=next;
  }

}

void deleteCorrectList(CorrectNode *list){
  /*
  Deletes a the list of corrected words
  */
  CorrectNode* next;
  while(list != NULL){
    free(list->word);
    next = list->next;
    free(list);
    list=next;
  }
}



void deleteSpecs(Specs* specs){
  /*
  Deletes a Specs struct, doing the apropriate frees.
  */
  //deleteSpecsList(specs->list);
  deleteCorrectList(specs->words);
  deleteSpecsList(specs->list);
  free(specs->id);
  free(specs);
}
