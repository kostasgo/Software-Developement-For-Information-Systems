#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"

WordBucketData* createBucketData(char * str, Clique* clique){
  WordBucketData *data= (WordBucketData*)malloc(sizeof(WordBucketData));
  data->id = strdup(str);
  data->clique = clique;
  data->flag=0;

  return data;
}

void deleteWordBucketData(WordBucketData* data){
  if(data->flag!=1){
    deleteClique(data->clique);

  }
  free(data->id);
  free(data);
}

WordBucket* createWordBucket(int size){

  WordBucket* bucket= (WordBucket*)malloc(sizeof(WordBucket));
  bucket->data =(WordBucketData**)malloc(sizeof(WordBucketData)*size);

  for(int i=0;i<size;i++){

    bucket->data[i] = createWordBucketData("-", NULL);


  }

  bucket->max =size;

  bucket->next = NULL;

  return bucket;

}



WordBucketData* searchWordBucket(WordBucket *b, char* str){
  for(int i=0;i<b->max;i++){
    //if the string is found, return its data
    if(!strcmp(b->data[i]->id,str)){
      return b->data[i];
    }
    //if "-" is found (sign for empty BucketData) return this empty data
    if(!strcmp(b->data[i]->id,"-")){
      return b->data[i];
    }
  }
  //if it reaches this point, then we must look in the next bucket
  if(b->next!=NULL){
    return searchBucket(b->next, str);
  }
  else{//create a new bucket and return its first element
    b->next=createBucket(b->max);
    return b->data[0];
  }

}


void printWordBucket(WordBucket* bucket){
  for(int i=0;i<bucket->max;i++){
    if(!strcmp(bucket->data[i]->id,"-" )){
      return;
    }
    printf("%s\n",bucket->data[i]->id);
  }
  if(bucket->next!=NULL){
    printf("Printing next bucket!\n");
    printBucket(bucket->next);
  }
}

void deleteWordBucket(WordBucket* b){
  WordBucket* next;
  while(b != NULL){
    for(int i=0;i<b->max;i++){
      deleteWordBucketData(b->data[i]);
    }
    free(b->data);
    next = b->next;
    free(b);
    b=next;
  }
}
