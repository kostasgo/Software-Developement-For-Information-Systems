#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"
#include "lowlevel-io.h"
#include "clique.h"
#include "parser.h"

#define DATAPATH "Datasets/2013_camera_specs"

int main(int argc, char* argv[]){
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

  Clique* cliques[numOfFiles];
  for(int i=0; i<numOfFiles; i++){
    cliques[i]=createClique();
  }

  for(int i=0;i<13;i++){

    Specs* specs= parser(directories[0], files[i]);
    insertClique(&(cliques[i]),specs);


  }

  for(int i=0;i<13;i++){

    printf("Clique %d:\n",i);
    printClique(cliques[i]);

  }
  printf("\n\n---------\n\n");

  printf("Merging 0 and 5!\n\n");
  mergeCliques(cliques[0],cliques[5]);
  for(int i=0;i<13;i++){

    printf("Clique %d:\n",i);
    printClique(cliques[i]);

  }
  printf("\n\n---------\n\n");

  printf("Merging 7 and 3!\n\n");
  mergeCliques(cliques[7],cliques[3]);
  mergeCliques(cliques[0],cliques[5]);
  for(int i=0;i<13;i++){

    printf("Clique %d:\n",i);
    printClique(cliques[i]);

  }
  printf("\n\n---------\n\n");

  printf("Merging 10 and 7s!\n\n");
  mergeCliques(cliques[10],cliques[7]);
  mergeCliques(cliques[0],cliques[5]);
  for(int i=0;i<13;i++){

    printf("Clique %d:\n",i);
    printClique(cliques[i]);

  }
  printf("\n\n---------\n\n");

  printf("Merging 11 and 12!\n\n");
  mergeCliques(cliques[11],cliques[12]);
  mergeCliques(cliques[0],cliques[5]);
  for(int i=0;i<13;i++){

    printf("Clique %d:\n",i);
    printClique(cliques[i]);

  }
  printf("\n\n---------\n\n");

  printf("Merging 0 and 10!\n\n");
  mergeCliques(cliques[0],cliques[10]);

  printf("\n\n---------\n\n");

  for(int i=0;i<13;i++){

    printf("Clique %d:\n",i);
    printClique(cliques[i]);

  }

  printf("\n\n---------\n\n");

  deletePath(path);
  deleteDirTable(files, numOfFiles);
  deleteDirTable(directories, numOfDirectories);

  for(int i=0;i<13;i++){
    deleteClique(cliques[i]);
  }
  return 0;


}
