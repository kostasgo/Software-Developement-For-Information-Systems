#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"


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

  insertBucket(&b, specs); //put it in there

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

Clique* searchHashtable(Hashtable* table, char* id){
  /*
    Searches inside the hashtable to find the spec
    with the given key, and returns its Clique.
  */

  int pos = compress(hash(id),table->max); //the position where it should be

  Clique* cl= searchBucket(id, table->array[pos]);
  return cl;

}

void printHashtable(Hashtable* table){

  for(int i=0;i<table->max;i++){
    printf("Bucket %d:\n",i);
    printBucket(table->array[i]);
  }
  printf("\n");
}
