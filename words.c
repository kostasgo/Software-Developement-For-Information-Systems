#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "words.h"
#include "math.h"

Word* createWord(char * str){
  Word *word= (Word*)malloc(sizeof(Word));
  word->str = strdup(str);
  word->counter = 0;
  word->index=-1;
  word->tf_sum=0;
  word->tfidf_score=0;
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

void updateTfIdfScore(VocBucket *b,int totalFiles){

  for(int i=0;i<b->max;i++){
    if(!strcmp(b->words[i]->str,"-")){
      return;
    }
    double avg_tf=b->words[i]->tf_sum/(double)totalFiles;
    double temp =(double)totalFiles/(double)b->words[i]->counter;
    double idf= log(temp);
    //printf("%lf %lf\n",temp,idf);
    b->words[i]->tfidf_score=avg_tf*idf;
  }
  //if it reaches this point, then we must look in the next bucket
  if(b->next!=NULL){
    updateTfIdfScore(b->next, totalFiles);
  }
}

Word* searchVocBucket(VocBucket *b, char* str){
  /*
  Search for a word in the bucket. If it finds an empty word it returns it.

  */
  for(int i=0;i<b->max;i++){
    //if the string is found, return its word
    if(!strcmp(b->words[i]->str,str)){
      return b->words[i];
    }
    //if "-" is found (sign for empty Word) return this empty Word
    if(!strcmp(b->words[i]->str,"-")){
      return b->words[i];
    }
  }
  //if it reaches this point, then we must look in the next bucket
  if(b->next!=NULL){
    return searchVocBucket(b->next, str);
  }
  else{//create a new bucket and return its first element
    b->next=createVocBucket(b->max);
    return b->words[0];
  }

}


void printVocBucket(VocBucket* bucket){
  for(int i=0;i<bucket->max;i++){
    if(!strcmp(bucket->words[i]->str,"-" )){
      return;
    }
    printf("%s| Score: %lf\n",bucket->words[i]->str, bucket->words[i]->tfidf_score);
  }
  if(bucket->next!=NULL){
    //printf("Printing next bucket!\n");
    printVocBucket(bucket->next);
  }
}

void deleteVocBucket(VocBucket* b){
  VocBucket* next;
  while(b != NULL){
    for(int i=0;i<b->max;i++){
      deleteWord(b->words[i]);
    }
    free(b->words);
    next = b->next;
    free(b);
    b=next;
  }
}
