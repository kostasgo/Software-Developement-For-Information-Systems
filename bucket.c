#include <stdio.h>
#include <stdlib.h>
#include "bucket.h"
#include "clique.h"

Bucket* createBucket(int size){

  Bucket* bucket= (Bucket*)malloc(sizeof(Bucket));
  bucket->cliques =(Clique**)malloc(sizeof(Clique)*size);

  for(int i=0;i<size;i++){
    bucket->cliques[i] = createClique();
  }

  bucket->max =size;
  bucket->next = NULL;

  return bucket;

}



void insertBucket(Bucket **b, Specs* specs){
  /*
  Inserts a spec to a bucket. It traverses the bucket's array of cliques, until it finds a clique that is empty
  Then it enters the spec there. Otherwise, it checks the next bucket. If all non-NULL buckets in the list have been
  checked, it creates a new one and inserts the specs to the first clique.
  */
  Bucket *temp= *b;
  while(temp!=NULL){
    for(int i=0;i<temp->max;i++){//look inside the bucket
      //printf("%d %d\n",i, temp->max);
      if( temp->cliques[i]->size==0 ){//if we find an empty clique we insert the specs to it

        insertClique(&(temp->cliques[i]),specs);

        return;
      }

    }
    temp=temp->next;
  }
  //at this point there is no space or next bucket so we need to make one
  temp= createBucket((*b)->max);
  insertClique(&(temp->cliques[0]),specs);
}

void deleteBucket(Bucket* b){
  Bucket* next;
  while(b != NULL){
    for(int i=0;i<b->max;i++){
      deleteClique(b->cliques[i]);
    }
    free(b->cliques);
    next = b->next;
    free(b);
    b=next;
  }
}

Clique* searchBucket(char* id, Bucket* b){
  for(int i=0;i<b->max;i++){
    //if the string is found, return its clique
    if(isInClique(id, b->cliques[i])){
      return b->cliques[i];
    }
  }
  //if it reaches this point, then we must look in the next bucket
  if(b->next!=NULL){
    return searchBucket(id, b->next);
  }
  else{// or create a new bucket and return its first element
    return NULL;
  }
}

void printBucket(Bucket* bucket){
  for(int i=0;i<bucket->max;i++){
    printClique(bucket->cliques[i]);
  }
  if(bucket->next!=NULL){
    printf("Next bucket!\n");
    printBucket(bucket->next);
  }
}
