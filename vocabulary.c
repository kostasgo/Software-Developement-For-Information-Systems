#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "specs.h"

void toLower(char* s){
  /*
  Converts a string to lowercase
  */
  for(int i=0; s[i]; i++){
    s[i]=tolower(s[i]);
  }
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
      while( (currentString = strsep(&(tempVal->str), " ")) != NULL){
        if (strlen(currentString) == 1){ continue; }
        if (isStopword(currentString, stopwords)){ continue; }
        toLower(currentString);
        insertWord(&(specs->words), currentString);
      }
      tempVal = tempVal->next;
    }
    temp = temp->next;

  }
  deleteSpecsList(specs->list);
}

int main(){
  char* test=strdup("HeLlo\t frIeNDS\nxD\n");
  toLower(test);
  printf("%s",test);
  char** stopwords=createStopWordsTable();


  printf("%d\n", isStopword(test, stopwords));
  printf("%d\n", isStopword("your", stopwords));
  deleteStopWordsTable(stopwords);
  free(test);
}
