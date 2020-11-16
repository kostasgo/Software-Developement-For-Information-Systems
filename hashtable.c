#include "hashtable.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"

Hashtable* createHashtable(int size){
  /*
  Creates and returns an empty hashtable of given size. A Hashtable
  created by this function should always be deleted by deleteHashtable().
  */

  Hashtable *table = (Hashtable*)malloc(sizeof(Hashtable));
  table->array=(Clique **)malloc(sizeof(Clique*)*size);
  table->max=size;

  for(int i=0;i<size;i++){
    table->array[i]=createClique();
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
  and then tries to put it in the first position that is empty.
  This way, each item is in clique alone.
  */


  int pos = compress(hash(specs->id),(*table)->max); //find the position to enter
  Clique *cl= (*table)->array[pos];//get its clique

  while(cl->size!=0){
    pos++;  //go to next position
    pos%=(*table)->max;  //wrap around table
    cl= (*table)->array[pos];
  }

  insertClique(&cl, specs);

}

void deleteHashtable(Hashtable* table){
  /*
    Deletes the given hashtable.
  */
  for(int i=0;i<table->max;i++){
      deleteClique(table->array[i]);
  }
  free(table->array);
  free(table);
  }

{
  /*
    Searches inside the hashtable to find the spec
    with the given key, and returns its position.
  */
  int counter =0;
  int pos = compress(hash(id),table->max); //the position where it should be
  Clique *cl= table->array[pos];//get its clique
  while(!isInClique(id, cl) && counter<table->max){
    pos++;  //go to next position
    pos%=table->max;  //wrap around table
    cl= table->array[pos];
  }
  if(counter!=table->max){
    return pos;
  }
  else{
    return -1;
  }

}

void printHashtable(Hashtable* table){

  for(int i=0;i<table->max;i++){
    printf("Clique %d:\n",i);
    printClique(table->array[i]);
  }
  printf("\n");
}
