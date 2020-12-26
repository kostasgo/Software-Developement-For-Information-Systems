#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "word.h"

Word* createWord(char * str){
  Word *word= (Word*)malloc(sizeof(Word));
  word->str = strdup(str);
  word->counter =0;

  return word;
}


void deleteWord(Word* word){
  free(word->str);
  free(word);
}

VocBucket* createVocBucket(int size){

  VocBucket* bucket= (VocBucket*)malloc(sizeof(VocBucket));
  bucket->words =(Word**)malloc(sizeof(Word)*size);

  for(int i=0;i<size;i++){

    bucket->words[i] = createWord("-");
  }

  bucket->max =size;

  bucket->next = NULL;

  return bucket;

}



Word* searchVocBucket(VocBucket *b, char* str){
  for(int i=0;i<b->max;i++){
    //if the string is found, return its word
    if(!strcmp(b->Word[i]->id,str)){
      return b->Word[i];
    }
    //if "-" is found (sign for empty Word) return this empty Word
    if(!strcmp(b->Word[i]->id,"-")){
      return b->Word[i];
    }
  }
  //if it reaches this point, then we must look in the next bucket
  if(b->next!=NULL){
    return searchBucket(b->next, str);
  }
  else{//create a new bucket and return its first element
    b->next=createBucket(b->max);
    return b->Word[0];
  }

}


void printVocBucket(VocBucket* bucket){
  for(int i=0;i<bucket->max;i++){
    if(!strcmp(bucket->Word[i]->id,"-" )){
      return;
    }
    printf("%s\n",bucket->Word[i]->id);
  }
  if(bucket->next!=NULL){
    printf("Printing next bucket!\n");
    printBucket(bucket->next);
  }
}

void deleteVocBucket(VocBucket* b){
  VocBucket* next;
  while(b != NULL){
    for(int i=0;i<b->max;i++){
      deleteWord(b->Word[i]);
    }
    free(b->Word);
    next = b->next;
    free(b);
    b=next;
  }
}
