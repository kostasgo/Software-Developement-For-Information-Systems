#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bucket.h"
#include "clique.h"

BucketData* createBucketData(char * str, Clique* clique){
  BucketData *data= (BucketData*)malloc(sizeof(BucketData));
  data->id = strdup(str);
  data->clique = clique;
  data->flag=0;

  return data;
}

void deleteBucketData(BucketData* data){
  if(data->flag!=1){
    deleteClique(data->clique);

  }
  free(data->id);
  free(data);
}

Bucket* createBucket(int size){

  Bucket* bucket= (Bucket*)malloc(sizeof(Bucket));
  bucket->data =(BucketData**)malloc(sizeof(BucketData)*size);

  for(int i=0;i<size;i++){

    bucket->data[i] = createBucketData("-", NULL);


  }

  //bzero(array, sizeof(BucketData)*size);
  bucket->max =size;

  bucket->next = NULL;

  return bucket;

}



BucketData* searchBucket(Bucket *b, char* str){
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

/*
void bucketInsert(Bucket **b, BucketData *input){
  Bucket *temp = *b;
  while(temp!=NULL){
    int i;
    for(i=0;i< (temp->max);i++){//look inside the bucket
    //printf("%d\n",i);
      if( !strcmp((temp->data[i]->id),"-" ) ){//if we find space we place the data
      //printf("Placing %s\n",input->name);
        temp->data[i]=input;
        return;
      }
      if( !strcmp((temp->data[i]->id),input->id ) ){
        //printf("Placing %s\n",input->name);
        temp->data[i]=input;
        return;
      }
    }
    temp=temp->next;
  }
  //at this point there is no space or next bucket so we need to make one
  temp= createBucket((*b)->max);
  temp->data[0] = input;
}
*/

void printBucket(Bucket* bucket){
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

void deleteBucket(Bucket* b){
  Bucket* next;
  while(b != NULL){
    for(int i=0;i<b->max;i++){
      deleteBucketData(b->data[i]);
    }
    free(b->data);
    next = b->next;
    free(b);
    b=next;
  }
}
