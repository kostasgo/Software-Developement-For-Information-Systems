#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <fcntl.h>
//#include <errno.h>
#include <dirent.h>

#define DIRSIZE 30

int countDirectories(char* inputDir){

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

char* newPath(char* inputDir){
  char* path;
  bzero(path, 27 + strlen(inputDir));
  memcpy(path, "Datasets/2013_camera_specs/", 27);
  memcpy(path + 27, inputDir, strlen(inputDir));
  return path;
}


char** getDirTable(int size, char* path){

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
    files[j] = (char*)malloc(DIRSIZE+1);
    //files[j]=strdup(direntp->d_name);
    strcpy(files[j],direntp->d_name);
    //printf("%s %s\n",countries_table[i], files[j]);
  }
  closedir(dir);

  return files;
}

void destroyDirTable(char** table, int size){

  for(int i=0;i<=size;i++){
    free(table[i]);
  }
  free(table);
}

int main(){
  int a = countDirectories("Datasets/2013_camera_specs");
  printf("%d\n",a);

  char** directories= NULL;
  directories = getDirTable(a, "Datasets/2013_camera_specs");
  for(int i=0;i<=a;i++){
    printf("%s\n",directories[i]);
  }
  destroyDirTable(directories, a);
  return 0;
}
