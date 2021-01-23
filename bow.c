#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "specs.h"
#include "hashtable.h"
#include "bow.h"
#include "words.h"


void toLower(char* s){
  /*
  Converts a string to lowercase
  */

  for(int i=0; s[i]; i++){
    s[i]=tolower(s[i]);
  }
}

void cleanString(char* s){
  char* needle;
  while( (needle = strstr(s, "\\n"))!=NULL ){
    needle[0]=' ';
    needle[1]=' ';
  }
  while( (needle = strstr(s, "\\u"))!=NULL ){
    needle[0]=' ';
    needle[1]=' ';
    needle[2]=' ';
    needle[3]=' ';
    needle[4]=' ';
    needle[5]=' ';
  }
  int i, j = 0;

  for (i = 0; i < strlen(s); i++){
	  if(ispunct(s[i])){
		  s[i] = ' ';
		  //j++;
	  }
    s[i]=tolower(s[i]);
  }

  //if(j < strlen(s))
	//s[j] = '\0';
}

char** createStopWordsTable(){
  /*
  Creates a table of all the stopwords in the stopwords.csv. Table should be deleted with
  deleteStopWordsTable().
  */
  char **stopwords=NULL;
  stopwords=(char**)malloc(119*sizeof(char*));
  FILE* fp;
  fp = fopen("stopwords.csv", "r");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  char *line = NULL;
  size_t len=0;
  int counter=0;

  getline(&line, &len, fp);
  //printf("line %d: %s\n", counter, line);
  char* found;
  while ( (found = strsep(&line, ",")) != NULL){
    stopwords[counter]=strdup(found);
    counter++;
  }

  free(line);
  fclose(fp);
  return stopwords;
}

void deleteStopWordsTable(char** stopwords){
  /*
  Deletes the table of stopwords
  */
  for(int i=0;i<119;i++){
    free(stopwords[i]);
  }
  free(stopwords);
}

int isStopword(char* str, char** stopwords){
  /*
  Examines if the given word is in the table of stopwords
  */

  for(int i=0; i<119; i++){

    //printf("%d\n", strcmp(str,stopwords[i]));
    if(!strcmp(str,stopwords[i])){
      return 1;
    }
  }
  return 0;
}

void filterSpec(Specs* specs, char** stopwords){
  /*
  Now we are filtering the Specs and putting everything into
  its -words- member
  */
  SpecsNode* temp= specs->list;
  while(temp!=NULL){
    Value* tempVal = temp->data->value;
    while(tempVal!=NULL){
      char* currentString;
      char* temp;
      cleanString(tempVal->str);
      while( (currentString = strsep(&(tempVal->str), " ")) != NULL){

        //toLower(currentString);

        if(strlen(currentString) <= 1){ continue; }
        if(isStopword(currentString, stopwords)){ continue; }
        if ((temp = malloc((strlen(currentString) + 1) * sizeof(char))) == NULL){
			       perror("malloc failed");
			       exit(EXIT_FAILURE);
		    }

        memcpy(temp, currentString, strlen(currentString));
        temp[strlen(currentString)] = '\0';
        currentString = strdup(temp);
        free(temp);
        insertCorrect(&specs, currentString);
        free(currentString);
      }
      tempVal = tempVal->next;
    }
    temp = temp->next;

  }
  
}


Vocabulary* createVocabulary(int size, int bucketSize){
  /*
  Creates and returns an empty Vocabulary Table of given size. A Vocabulary
  created by this function should always be deleted by deleteVocabulary().
  */

  Vocabulary *table = (Vocabulary*)malloc(sizeof(Vocabulary));
  table->array=(VocBucket **)malloc(sizeof(VocBucket*)*size);
  table->max=size;
  table->total=0;

  for(int i=0;i<size;i++){
    table->array[i]=createVocBucket(bucketSize);
  }
  return table;
}


void insertVocabulary(Vocabulary **table, char* str, double tf){
  /*
  Inserts a word into the Vocabulary Table. Finds the right position
  and then tries to put it in the VocBucket. If it already exists,
  we just increase the words counter.
  */

  int pos = compress(hash(str),(*table)->max); //find the position to enter
  VocBucket *b= (*table)->array[pos];//get its VocBucket

  Word *word = searchVocBucket(b, str);//find the first available space in the VocBucket

  word->counter++;
  word->tf_sum+=tf;
  //printf("%lf\n",word->tf_sum);
  if(!strcmp(word->str,str)){ //if the word was actually found just increase the counter

    return;
  }

  //Otherwise, an empty word was returned.
  (*table)->total++;
  free(word->str);
  word->str=strdup(str); //rename the Word

}

int fillVocabulary(Vocabulary **table, ListNode* specsList){
  /*
  Updates the vocabulary by entering each word that is found
  in the entirety of the Dataset X. Returns the number of json
  files in the Dataset.
  */
  ListNode* node=specsList;
  int totalSpecs=0;
  while(node!=NULL){
    CorrectNode *current=node->specs->words;
    int total = node->specs->total;
    while(current!=NULL){

      double tf=(double)current->counter/(double)total; //calculate the word's tf
      //printf("%lf\n",tf);
      current->tf=tf;
      insertVocabulary(table, current->word, tf);
      current=current->next;


    }
    totalSpecs++;
    node=node->next;
  }
  return totalSpecs;
}

void updateScores(Vocabulary* table, int totalFiles){
  for(int i=0;i<table->max;i++){
    updateTfIdfScore(table->array[i],totalFiles);
  }

}

Word** shrinkTable(Vocabulary* table, double margin, int* size){
  /*
  Runs the Vocabulary table and changes the index of every word
  that has a better score than the margin to a number, starting from 0.
  Then creates a table of words, where each word is in the position that their index
  indicates.
  */
  int counter=0;
  for(int i=0;i<table->max;i++){
    VocBucket* b=table->array[i];
    while(b!=NULL){
      for(int j=0;j<b->max;j++){
        if(!strcmp(b->words[j]->str,"-")){
          break;
        }
        if(b->words[j]->tfidf_score>=margin){
          b->words[j]->index=counter;

          counter++;
        }
      }
      b=b->next;
    }

  }
  *size=counter;
  Word** bow=(Word**)malloc(sizeof(Word*)*counter);

  for(int i=0;i<table->max;i++){
    VocBucket* b=table->array[i];
    while(b!=NULL){
      for(int j=0;j<b->max;j++){
        if(!strcmp(b->words[j]->str,"-")){
          break;
        }
        if(b->words[j]->index!=-1){
          bow[b->words[j]->index]=b->words[j];

        }
      }
      b=b->next;
    }

  }
  return bow;
}


void deleteVocabulary(Vocabulary* table){
    /*
      Deletes the given Vocabulary.
    */
  for(int i=0;i<table->max;i++){
    deleteVocBucket(table->array[i]);
  }
  free(table->array);
  free(table);
}

Word* searchVocabulary(Vocabulary* table, char* str){
  /*
    Searches inside the Vocabulary to find the spec
    with the given key, and returns the cell where the clique is.
  */

  int pos = compress(hash(str),table->max); //the position where it should be


  Word* word= searchVocBucket(table->array[pos], str);

  return word;

}


void printVocabulary(Vocabulary* table){

  for(int i=0;i<table->max;i++){
    printf("Bucket %d:\n",i);
    printVocBucket(table->array[i]);
  }
  printf("\n");
}
