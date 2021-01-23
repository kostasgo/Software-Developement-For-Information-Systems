/*
This is a project created for the module "Software Developement for Information Systems"
for the Univercity of Athens, Department of Informatics.

Authors: Konstantinos Gkogkas, Nikolaos Sentis

Compile: make
Run: ./modelTraining
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "specs.h"
#include "lowlevel-io.h"
#include "clique.h"
#include "parser.h"
#include "hashtable.h"
#include "create-output.h"
#include "linkedlist.h"
#include "bow.h"
#include "words.h"
#include "logistic_regression.h"

#define LARGE "Datasets/sigmod_large_labelled_dataset.csv"
#define MEDIUM "Datasets/sigmod_medium_labelled_dataset.csv"

#define LARGELINES 297651
#define MEDIUMLINES 46665

#define DATAPATH "Datasets/2013_camera_specs"

#define HASHTABLE_SIZE 30000
#define BUCKET_SIZE 4
#define VOCABULARY_TABLE_SIZE 10000
#define VOCABULARY_BUCKETSIZE 6

#define MARGIN 0.0029

#define LEARNING_RATE 0.001

#define TRAINING_PERCENT 60
#define VALIDATION_PERCENT 20

int main(int argc, char* argv[]){

  char* inputFile;
  int noOfLines;


  if(argc!=2 && argc!=1){
    printf("Usage: ./modelTraining (-l)\n");
    return -1;
  }
  if(argc == 1){

    inputFile=strdup(MEDIUM);
    noOfLines=MEDIUMLINES;
    printf("Running with medium file! (Default)\n\n");
  }
  if(argc == 2){
    if(!strcmp(argv[1],"-l")){

      inputFile=strdup(LARGE);
      noOfLines=LARGELINES;
      printf("Running with large file!\n\n");
    }
    else{
      printf("Usage: ./modelTraining (-l)\n");
      return -1;
    }

  }
/*
Clique* cl1= createClique();
Clique* cl2= createClique();
Clique* cl3= createClique();
Clique* cl4= createClique();

Specs* specs;

specs=NULL;
specs=parser("buy.net","4233.json");
insertClique(&cl1, specs);
specs=NULL;
specs=parser("buy.net","4236.json");
insertClique(&cl1, specs);
specs=NULL;
specs=parser("buy.net","4239.json");
insertClique(&cl1, specs);


specs=NULL;
specs=parser("buy.net","4247.json");
insertClique(&cl2, specs);
specs=NULL;
specs=parser("buy.net","4255.json");
insertClique(&cl2, specs);
specs=NULL;
specs=parser("buy.net","4258.json");
insertClique(&cl2, specs);

specs=NULL;
specs=parser("buy.net","4261.json");
insertClique(&cl3, specs);
specs=NULL;
specs=parser("buy.net","4266.json");
insertClique(&cl3, specs);

specs=NULL;
specs=parser("buy.net","4272.json");
insertClique(&cl4, specs);



insertNegatives(&(cl2->negatives), cl3);
insertNegatives(&(cl3->negatives), cl2);


char** tempSet;
int totalSize=0;
char** returnSet=(char**)malloc(sizeof(char*)*(totalSize));
int size,oldsize;

size=-1;
tempSet=getCliquePairs(cl1, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);

size=-1;
tempSet=getAllNegatives(cl1, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);


size=-1;
tempSet=getCliquePairs(cl2, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);

size=-1;
tempSet=getAllNegatives(cl2, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);


size=-1;
tempSet=getCliquePairs(cl3, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);

size=-1;
tempSet=getAllNegatives(cl3, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);

size=-1;
tempSet=getCliquePairs(cl4, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);

size=-1;
tempSet=getAllNegatives(cl4, &size);
oldsize=(totalSize);
(totalSize)+=size;
returnSet=(char**)realloc(returnSet, sizeof(char*)*(totalSize));
for(int k=oldsize; k<(totalSize); k++){
  returnSet[k]=strdup(tempSet[k-oldsize]);
}
free(tempSet);

for(int i=0; i<totalSize; i++){
  printf("%s\n",returnSet[i]);
}


  int lineToStop=noOfLines*LEARN_PERCENT/100;


  char **csv = (char**)malloc(sizeof(char*)*noOfLines);



  FILE *fp;
  fp = fopen(inputFile, "r");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  char *line = NULL;
  size_t len=0;
  getline(&line, &len, fp);
  int c=0;
  while(getline(&line, &len, fp) != -1){
    //printf("%d\n",c);
    line[strlen(line)-1]='\0';
    csv[c]=strdup(line);
    c++;

  }
  free(line);

  FILE *fp2;
  fp2 = fopen("testing.csv", "w");
  if(fp2 == NULL){
    perror("Unable to open file!");
    exit(1);
  }

  shuffleArray(csv,noOfLines);

  for(int i=lineToStop;i<noOfLines;i++){
    //printf("%s\n",csv[i]);
    fprintf(fp2,"%s\n",csv[i]);
  }

*/

  char** stopwords= createStopWordsTable();
  ListNode* specsList=NULL;
  Hashtable* cliques = createHashtable(HASHTABLE_SIZE, BUCKET_SIZE);
  Vocabulary* vocabulary = createVocabulary(VOCABULARY_TABLE_SIZE, VOCABULARY_BUCKETSIZE);


  //Access Files
  int numOfFiles, numOfDirectories;
  //Create Directories table

  numOfDirectories=countDirectories(DATAPATH);
  char** directories = createDirTable(numOfDirectories, DATAPATH);
  printf("Processing directories...\n\n");

  for(int i=0; i<numOfDirectories; i++){
    //For each directory create files table

    printf("%s\n",directories[i]);
    char* path= createPath(DATAPATH, directories[i]);
    int numOfFiles=countDirectories(path);
    char ** files=createDirTable(numOfFiles, path);
    Specs* specs=NULL;
    for(int j=0;j<numOfFiles;j++){
      //printf("%s\n\n",files[j]);
      //Parse each .json file and insert it into the hashtable
      specs= parser(directories[i],files[j]);

      filterSpec(specs, stopwords);
      insertList(&specsList, specs);
      insertHashtable(&cliques,specs);
      specs=NULL;
    }

    deleteDirTable(files, numOfFiles);
    deletePath(path);
  }
  deleteStopWordsTable(stopwords);
  deleteDirTable(directories, numOfDirectories);

  // Here we create the vocabulary
  printf("\nCreating Bag Of Words...\n");
  int totalSpecs=fillVocabulary(&vocabulary, specsList);

  updateScores(vocabulary, totalSpecs);
  //printVocabulary(vocabulary);
  int bowSize=-1;
  Word** bow=shrinkTable(vocabulary,MARGIN, &bowSize);
  Classifier* logReg= createClassifier(bowSize*2+1,LEARNING_RATE);
  //printf("%d\n",size);

  for(int i=0; i<bowSize;i++){
    printf("%d %s\n",i, bow[i]->str);
  }


  printf("\nGoing through %s...\n",inputFile);

  FILE *fp;
  fp = fopen(inputFile, "r");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }

	FILE *fp2;
  fp2= fopen("tempSet.csv", "w");
  if(fp2 == NULL){
    perror("Unable to open file!");
    exit(1);
  }
/*
  printf("\nTraining the model using %d%% of the given file...\n",LEARN_PERCENT);
  int linesTested=0;
  //parse the csv to train and test the model
*/

  char *line = NULL;
  size_t len=0;
  getline(&line, &len, fp);

  //printHashtable(cliques);
  printf("Adjusting cliques...\n");
  while(getline(&line, &len, fp) != -1){
    parseCsv(line, cliques, vocabulary, bowSize, logReg);

  }
  fclose(fp);
  //printHashtable(cliques);



  printf("Creating final set...\n");
  int totalSize=-1;
  int skipcount=0;
  char** tempSet = createFinalSet(cliques, &totalSize, &skipcount);

  for(int i=0; i<totalSize; i++){
    fprintf(fp2,"%s\n",tempSet[i]);
  }
  fclose(fp2);

  for(int i=0; i<totalSize; i++){
    free(tempSet[i]);
  }
  free(tempSet);


  char command[100];
  printf("deleting double 0s\n");
  system("sort tempSet.csv | uniq > finalSet.csv");

  //printf("totalsize : %d\n",totalSize);

  FILE *fp3;
  fp3= fopen("finalSet.csv", "r");
  if(fp3 == NULL){
    perror("Unable to open file!");
    exit(1);
  }

  line=NULL;
  len=0;
  int finalSize=0;
  while(getline(&line, &len, fp3) != -1){
    finalSize++;
    //printf("%s\n",line);
  }

  int trainingSize=finalSize*TRAINING_PERCENT/100;
  int validationSize=finalSize*VALIDATION_PERCENT/100;
  int testingSize=finalSize-(trainingSize+validationSize);


  char** trainingSet=(char**)malloc(sizeof(char*)*trainingSize);
  char** validationSet=(char**)malloc(sizeof(char*)*validationSize);
  char** testingSet=(char**)malloc(sizeof(char*)*testingSize);

  int c=0;
  while(getline(&line, &len, fp3) != -1){
    line[strlen(line)-1]='\0';
    if(c<trainingSize){
      trainingSet[c]=strdup(line);
    }
    else if(c>=trainingSize && c<(trainingSize+validationSize)){
      validationSet[c-trainingSize]=strdup(line);
    }
    else{
      testingSet[c-trainingSize-testingSize]=strdup(line);
    }
    //finalSize++;
    //printf("%s\n",line);
}

  fclose(fp3);
  for(int i=0; )
  FILE *fp4;
  fp4= fopen("statistics", "w");
  if(fp4 == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  printf("\nFinished! Run ./validation.sh to see how you did!\n");
  fprintf(fp4, "STATISTICS\n\n\n\nFILE USED:");
  if(!strcmp(inputFile,LARGE)){
    fprintf(fp4, "LARGE\n");
  }
  else{
    fprintf(fp4, "MEDIUM\n");
  }

  fprintf(fp4, "\nPERCENTAGE OF FILE USED FOR TRAINING: %d\n\nPERCENTAGE OF FILE USED FOR VALIDATION: %d\n\nLEARNING RATE: %lf\n\nMARGIN OF AVERAGE TF-IDFs ALLOWED: %lf\n\n",TRAINING_PERCENT,VALIDATION_PERCENT,LEARNING_RATE,MARGIN);

  fprintf(fp4, "FINAL VOCABULARY: \n--------------------------------------------\n");
  for(int i=0;i<bowSize;i++){
    fprintf(fp4, "%s\n", bow[i]->str);

  }
  free(bow);
  fprintf(fp4, "--------------------------------------------\nWEIGHTS: \n--------------------------------------------\n");
  for(int i=0;i<=2*bowSize;i++){
    fprintf(fp4, "%lf\n", logReg->w[i]);

  }
  free(inputFile);

  deleteVocabulary(vocabulary);

  deleteHashtable(cliques);
  deleteList(specsList);
  deleteClassifier(logReg);/*

  for(int i=0; i<noOfLines; i++){
    free(csv[i]);
  }
  free(csv);*/

  fclose(fp4);

  return 0;


}
