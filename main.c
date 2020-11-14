#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"
#include "lowlevel-io.h"

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


  for(int i=0;i<numOfFiles;i++){

    char* temp=createPath(path, files[i]);
    printf("%s\n",temp);
    deletePath(temp);
  }
  deletePath(path);
  deleteDirTable(files, numOfFiles);
  deleteDirTable(directories, numOfDirectories);

  return 0;
}
