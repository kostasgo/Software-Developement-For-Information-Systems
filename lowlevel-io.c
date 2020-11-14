#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>



int countDirectories(char* inputDir){
  /*
  Counts and returns the number of sub-directories inside
  a directory. Can also count number of files inside a folder.
  */

  DIR * dir;
  struct 	dirent* direntp;
  int counter =0;


  // Count number of sub-Directories
  if ((dir = opendir(inputDir)) == NULL){
    fprintf(stderr, "Cannot open %s \n",inputDir);
  }
  else{
    //printf("test\n");
    while ((direntp=readdir(dir) ) != NULL ){
      if  (strcmp(".",direntp->d_name)&&strcmp("..",direntp->d_name)){
        //printf("%s\n",direntp->d_name);
        counter++;
      }
    }
      closedir(dir);
  }

  return counter;
}

char* createPath(char* dir, char* subdir){
  /*
  Creates a path to the new subdir. This string should be
  deleted by deletePath().
  */

  char* path = (char*) malloc(strlen(dir) + strlen(subdir) + 2);
  bzero(path, strlen(dir) + strlen(subdir) + 2);
  memcpy(path, dir, strlen(dir));
  memcpy(path + strlen(dir), "/", 1);
  memcpy(path + strlen(dir) + 1, subdir, strlen(subdir));
  return path;
}

void deletePath(char* path){
  /*
  Functions like this seem redundat, but it's to avoid
  the use of free() inside random functions and the main program,
  leading to a messier and more difficult to read code.
  */
  free(path);
}


char** createDirTable(int size, char* path){
  /*
  Creates and returns a table of strings, that is a table of the names directories or files
  found inside the given path.
  */
  DIR * dir;
  char **files=NULL;
  files=(char**)malloc(size*sizeof(char*));
  struct 	dirent* direntp;



  if ((dir = opendir(path)) == NULL){
    fprintf(stderr, "Cannot open %s \n",path);
  }

  for(int j=0;j<size;j++){
    direntp=readdir(dir);
    //printf("test i: %d j: %d\n", i, j);
    if  (!strcmp(".",direntp->d_name) || !strcmp("..",direntp->d_name)){
      j--;
      continue;
    }
    //files[j] = (char*)malloc(DIRSIZE+1);
    files[j]=strdup(direntp->d_name);
    //strcpy(files[j],direntp->d_name);
    //printf("%s %s\n",countries_table[i], files[j]);
  }
  closedir(dir);

  return files;
}

void deleteDirTable(char** table, int size){
  /*
  Deletes the table of strings, by doing the apropriate frees.
  */
  for(int i=0;i<size;i++){
    free(table[i]);
  }
  free(table);
}
