#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "specs.h"
#include "hashtable.h"
#include "vocabulary.h"
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
  
  int i, j = 0;
  
  for (i = 0; i < strlen(s); i++){
	  if(isalnum(s[i])){
		  s[j] = s[i];
		  j++;
	  }
  }
  
  if(j < strlen(s))
	s[j] = '\0';
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
      
      while( (currentString = strsep(&(tempVal->str), " ")) != NULL){

        toLower(currentString);
        cleanString(currentString);
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
      }
      tempVal = tempVal->next;
    }
    temp = temp->next;

  }
  deleteSpecsList(specs->list);
}


VocTable* createVocTable(int size, int bucketSize){
  /*
  Creates and returns an empty Vocabulary Table of given size. A VocTable
  created by this function should always be deleted by deleteVocTable().
  */

  VocTable *table = (VocTable*)malloc(sizeof(VocTable));
  table->array=(VocBucket **)malloc(sizeof(VocBucket*)*size);
  table->max=size;
  table->total=0;

  for(int i=0;i<size;i++){
    table->array[i]=createVocBucket(bucketSize);
  }
  return table;
}


void insertVocTable(VocTable **table, char *str){
  /*
  Inserts a word into the Vocabulary Table. Finds the right position
  and then tries to put it in the VocBucket. If it already exists, we just increase the words counter
  */


  int pos = compress(hash(str),(*table)->max); //find the position to enter
  VocBucket *b= (*table)->array[pos];//get its VocBucket

  Word *word = searchVocBucket(b, str);//find the first available space in the VocBucket

  if(!strcmp(word->str,str)){ //if the word was actually found just increase the counter
    word->counter++;
    return;
  }

  //Otherwise, an empty word was returned.
  (*table)->total++;
  free(word->str);
  word->str=strdup(str); //rename the Word to the file's id
  word->counter++;


}

void fillVocabulary(VocTable **table, ListNode* specsList){
  ListNode* node=specsList;
  while(node!=NULL){
    CorrectNode *current=node->specs->words;
    while(current!=NULL){
      insertVocTable(table, current->word);
      current=current->next;

    }
    node=node->next;
  }
}

void deleteVocTable(VocTable* table){
    /*
      Deletes the given VocTable.
    */
  for(int i=0;i<table->max;i++){
    deleteVocBucket(table->array[i]);
  }
  free(table->array);
  free(table);
}

Word* searchVocTable(VocTable* table, char* str){
  /*
    Searches inside the VocTable to find the spec
    with the given key, and returns the cell where the clique is.
  */

  int pos = compress(hash(str),table->max); //the position where it should be


  Word* word= searchVocBucket(table->array[pos], str);

  return word;

}


void printVocTable(VocTable* table){

  for(int i=0;i<table->max;i++){
    printf("VocBucket %d:\n",i);
    printVocBucket(table->array[i]);
  }
  printf("\n");
}
