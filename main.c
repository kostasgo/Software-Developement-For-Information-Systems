#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"
#include "lowlevel-io.h"
#include "clique.h"
#include "parser.h"
#include "hashtable.h"
#include "create-output.h"

#define LARGE "Datasets/sigmod_large_labelled_dataset.csv"
#define MEDIUM "Datasets/sigmod_medium_labelled_dataset.csv"

#define DATAPATH "Datasets/2013_camera_specs"

#define HASHTABLE_SIZE 32000

int main(int argc, char* argv[]){
/*
  //TESTING specs.c
  char* dir= strdup("www.ebay.com");
  char* f= strdup("32000.json");
  char* spec;
  spec=createSpecsID(dir,f);

  printf("%s\n",spec);
  free(spec);

//TESTING lowlvel-io,c
  int numOfDirectories=countDirectories(DATAPATH);
  char ** directories=createDirTable(numOfDirectories, DATAPATH);
  char* path= createPath(DATAPATH, directories[0]);
  int numOfFiles=countDirectories(path);
  char ** files=createDirTable(numOfFiles, path);

  printf("NUM OF FILES: %d", numOfFiles);

  Hashtable* cliques=createHashtable(13);


  for(int i=0;i<cliques->max;i++){
    Specs* specs= parser(directories[0],files[i]);
    insertHashtable(&cliques,specs);
  }

  printHashtable(cliques);
  printf("\n\n---------\n\n");

  printf("Merging 0 and 5!\n\n");
  mergeCliques(cliques->array[0],cliques->array[5]);
  printHashtable(cliques);
  printf("\n\n---------\n\n");

  printf("Merging 7 and 3!\n\n");
  mergeCliques(cliques->array[7],cliques->array[3]);

  printHashtable(cliques);
  printf("\n\n---------\n\n");

  printf("Merging 10 and 7s!\n\n");
  mergeCliques(cliques->array[10],cliques->array[7]);

  printHashtable(cliques);
  printf("\n\n---------\n\n");

  printf("Merging 11 and 12!\n\n");
  mergeCliques(cliques->array[11],cliques->array[12]);

  printHashtable(cliques);
  printf("\n\n---------\n\n");

  printf("Merging 0 and 10!\n\n");
  mergeCliques(cliques->array[0],cliques->array[10]);

  printf("\n\n---------\n\n");

  printHashtable(cliques);

  printf("\n\n---------\n\n");

  deletePath(path);
  deleteDirTable(files, numOfFiles);
  deleteDirTable(directories, numOfDirectories);

  deleteHashtable(cliques);
  free(dir);
  free(f);

  */

  char* inputFile;
  for (int i=0;i<argc; i++){
    printf("%d: %s\n",i, argv[i]);
  }

  if(argc!=2 && argc!=1){
    printf("Usage: ./disambugator (-l)\n");
    return -1;
  }
  if(argc == 1){

    inputFile=strdup(MEDIUM);
  }
  if(argc == 2){
    if(!strcmp(argv[1],"-l")){

      inputFile=strdup(LARGE);
    }
    else{
      printf("Usage: ./disambugator (-l)\n");
      return -1;
    }

  }

  Hashtable* cliques = createHashtable(HASHTABLE_SIZE);

  //Access Files
  int numOfFiles, numOfDirectories;
  //Create Directories table
  numOfDirectories=countDirectories(DATAPATH);
  char** directories = createDirTable(numOfDirectories, DATAPATH);

  for(int i=0; i<numOfDirectories; i++){
    //For each directory create files table
    char* path= createPath(DATAPATH, directories[i]);
    int numOfFiles=countDirectories(path);
    char ** files=createDirTable(numOfFiles, path);
    Specs* specs=NULL;
    for(int j=0;j<numOfFiles;j++){
      //Parse each .json file and insert it into the hashtable
      specs= parser(directories[i],files[j]);
      insertHashtable(&cliques,specs);
      specs=NULL;
    }

    deleteDirTable(files, numOfFiles);
  }

  deleteDirTable(directories,numOfDirectories);
  deleteHashtable(cliques);

  FILE *fp;
  fp = fopen(inputFile, "r");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }
  char *line = NULL;
  size_t len=0;

  while(getline(&line, &len, fp) != -1){
      adjustCliques(line, &cliques);

  }
  free(line);


  return 0;


}
