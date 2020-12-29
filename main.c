/*
This is a project created for the module "Software Developement for Information Systems"
for the Univercity of Athens, Department of Informatics.

Authors: Konstantinos Gkogkas, Nikolaos Sentis

Compile: make
Run: ./disambugator
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

#define LARGELINES 297653
#define MEDIUMLINES 46667

#define DATAPATH "Datasets/2013_camera_specs"

#define HASHTABLE_SIZE 30000
#define BUCKET_SIZE 4
#define VOCABULARY_TABLE_SIZE 10000
#define VOCABULARY_BUCKETSIZE 6

#define MARGIN 0.0029

#define LEARNING_RATE 0.001

#define LEARN_PERCENT 80

int main(int argc, char* argv[]){

  char* inputFile;
  int noOfLines;


  if(argc!=2 && argc!=1){
    printf("Usage: ./disambugator (-l)\n");
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
      printf("Usage: ./disambugator (-l)\n");
      return -1;
    }

  }

  int lineToStop=noOfLines*LEARN_PERCENT/100;
  printf("%d\n",lineToStop);
  char** stopwords= createStopWordsTable();
  ListNode* specsList=NULL;
  Hashtable* cliques = createHashtable(HASHTABLE_SIZE, BUCKET_SIZE);
  Vocabulary* vocabulary = createVocabulary(VOCABULARY_TABLE_SIZE, VOCABULARY_BUCKETSIZE);


  //Access Files
  int numOfFiles, numOfDirectories;
  //Create Directories table

  numOfDirectories=countDirectories(DATAPATH);
  char** directories = createDirTable(numOfDirectories, DATAPATH);
  printf("processing directories...\n\n");

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

  printf("\nCreating Bag Of Words...\n");
  int totalSpecs=fillVocabulary(&vocabulary, specsList);

  updateScores(vocabulary, totalSpecs);
  //printVocabulary(vocabulary);
  int bowSize=-1;
  Word** bow=shrinkTable(vocabulary,MARGIN, &bowSize);
  Classifier* logReg= createClassifier(bowSize*2+1,LEARNING_RATE);
  //printf("%d\n",size);

  for(int i=0; i<bowSize;i++){
    printf("%d %s\t%lf\n",i, bow[i]->str, bow[i]->tfidf_score);
  }
  FILE *fp;
  fp = fopen(inputFile, "r");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  char *line = NULL;
  size_t len=0;
  int c=0;
  printf("\nGoing through %s...\n",inputFile);


	FILE *fp2;
  fp2= fopen("predictions.csv", "w");
  if(fp2 == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  getline(&line, &len, fp); // read 1st line
  printf("Training the model...\n");
  int linesTested=0;
  while(getline(&line, &len, fp) != -1){
    //printf("%d\n",c);
    if(c==lineToStop){
      printf("Doing predictions...\n");
      linesTested=0;
    }
    parseCsv(line, cliques, vocabulary, bowSize, logReg, c, fp2, lineToStop);
    c++;
    linesTested++;
  }

  //printHashtable(cliques);
  //printf("Num of lines: %d\n",c);
  printf("\nFinished! Lines tested: %d\n",linesTested);
  free(line);

  free(inputFile);
  outputToFile(cliques);
  deleteVocabulary(vocabulary);

  deleteHashtable(cliques);
  deleteList(specsList);


  fclose(fp);
  fclose(fp2);

  return 0;


}
