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

#define LARGE "Datasets/sigmod_large_labelled_dataset.csv"
#define MEDIUM "Datasets/sigmod_medium_labelled_dataset.csv"

#define DATAPATH "Datasets/2013_camera_specs"

#define HASHTABLE_SIZE 30000
#define BUCKET_SIZE 4

int main(int argc, char* argv[]){

  char* inputFile;


  if(argc!=2 && argc!=1){
    printf("Usage: ./disambugator (-l)\n");
    return -1;
  }
  if(argc == 1){

    inputFile=strdup(MEDIUM);
    printf("Running with medium file! (Default)\n");
  }
  if(argc == 2){
    if(!strcmp(argv[1],"-l")){

      inputFile=strdup(LARGE);
      printf("Running with large file!\n");
    }
    else{
      printf("Usage: ./disambugator (-l)\n");
      return -1;
    }

  }
  ListNode* specsList=NULL;
  Hashtable* cliques = createHashtable(HASHTABLE_SIZE, BUCKET_SIZE);

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
      insertList(&specsList, specs);
      insertHashtable(&cliques,specs);
      specs=NULL;
    }

    deleteDirTable(files, numOfFiles);
    deletePath(path);
  }

  deleteDirTable(directories, numOfDirectories);


//  Specs* specs=parser("www.alibaba.com","34956.json");
//  printSpecs(specs);

  FILE *fp;
  fp = fopen(inputFile, "r");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  char *line = NULL;
  size_t len=0;
  int c=0;
  while(getline(&line, &len, fp) != -1){
    //printf("%s\n",line);
    adjustCliques(line, cliques);
    c++;
  }

  //printHashtable(cliques);
  printf("Num of lines: %d\n",c);
  free(line);

  free(inputFile);
  outputToFile(cliques);
  deleteList(specsList);
  deleteHashtable(cliques);


  fclose(fp);

  return 0;


}
