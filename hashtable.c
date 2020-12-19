#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"
#include "clique.h"


Hashtable* createHashtable(int size, int bucketSize){
  /*
  Creates and returns an empty hashtable of given size. A Hashtable
  created by this function should always be deleted by deleteHashtable().
  */

  Hashtable *table = (Hashtable*)malloc(sizeof(Hashtable));
  table->array=(Bucket **)malloc(sizeof(Bucket*)*size);
  table->max=size;

  for(int i=0;i<size;i++){
    table->array[i]=createBucket(bucketSize);
  }
  return table;
}

unsigned long hash(unsigned char *str){
  /*
  djb2: This algorithm was first reported by Dan Bernstein. The use of
  33 helps to produce widely different outputs.
  */
  unsigned long hash = 5381;
  int c;

  while (c = *str++)
      hash = ((hash << 5) + hash) + c;

  return hash;
}

int compress(unsigned long key, int size){
  /*
  Compresses the key to the size of the hashtable.
  */
  return key % size;
}

void insertHashtable(Hashtable **table, Specs *specs){
  /*
  Inserts a Specs item into the hashtable. Finds the right position
  and then tries to put it in the bucket.
  */


  int pos = compress(hash(specs->id),(*table)->max); //find the position to enter
  Bucket *b= (*table)->array[pos];//get its bucket
  BucketData *input = searchBucket(b, specs->id);//find the first available space in the bucket

  Clique* cl= createClique();
  insertClique(&cl, specs); //create the clique of the file

  free(input->id);
  input->id=strdup(specs->id); //rename the BucketData to the file's id
  input->clique=cl;


}

void deleteHashtable(Hashtable* table){
  /*
    Deletes the given hashtable.
  */
  for(int i=0;i<table->max;i++){
    deleteBucket(table->array[i]);
  }
  free(table->array);
  free(table);
  }

BucketData* searchHashtable(Hashtable* table, char* id){
  /*
    Searches inside the hashtable to find the spec
    with the given key, and returns the cell where the clique is.
  */

  int pos = compress(hash(id),table->max); //the position where it should be


  BucketData* data= searchBucket(table->array[pos], id);

  return data;

}

void mergeCliques(Hashtable* table, char* id1, char* id2){

  BucketData* data1=searchHashtable(table, id1);
	BucketData* data2=searchHashtable(table, id2);

  if(compareCliques(data1->clique,data2->clique)){return;}

  if(data1->clique==NULL){
		printf("%s not in hashtable\n",id1);
		return;
	}
	if(data2->clique==NULL){
		printf("%s not in hashtable\n",id2);
		return;
	}
  BucketData *new, *old;
  if(data1->clique->size>=data2->clique->size){
    new = data1;
    old = data2;
  }
  else{
    new = data2;
    old = data1;
  }

  Clique * temp=old->clique;
  concatCliqueList(new->clique->list,temp->list);
  new->clique->size=temp->size + new->clique->size;



  CliqueNode* node = old->clique->list;
  //printClique(old->clique);
  //printf("Size is: %d\n\n",old->clique->size);
  int size = old->clique->size;
  for(int i=0; i<size; i++){
    //printf("%d\n",i);
    int pos_temp=-1;
    BucketData* temp_data = searchHashtable(table, node->specs->id);
    //deleteClique(temp_data->clique);
    temp_data->clique=new->clique;
    temp_data->flag=1;
    node=node->next;

  }

}

void printHashtable(Hashtable* table){

  for(int i=0;i<table->max;i++){
    printf("Bucket %d:\n",i);
    printBucket(table->array[i]);
  }
  printf("\n");
}
