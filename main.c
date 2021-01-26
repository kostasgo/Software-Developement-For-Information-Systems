/*
This is a project created for the module "Software Developement for Information Systems"
for the Univercity of Athens, Department of Informatics.

Authors: Konstantinos Gkogkas, Nikolaos Sentis, Konstantinos Maragkos

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
#include "thread_pool.h"

#define LARGE "Datasets/sigmod_large_labelled_dataset.csv"
#define MEDIUM "Datasets/sigmod_medium_labelled_dataset.csv"

#define LARGELINES 297651
#define MEDIUMLINES 46665

#define DATAPATH "Datasets/2013_camera_specs"

#define HASHTABLE_SIZE 30000
#define BUCKET_SIZE 4
#define VOCABULARY_TABLE_SIZE 10000
#define VOCABULARY_BUCKETSIZE 6

#define MARGIN 0.0016

#define LEARNING_RATE 0.001

#define TRAINING_PERCENT 60
#define VALIDATION_PERCENT 20

#define QUEUE_SIZE 1000
#define THREAD_NUM 64

#define BATCHSIZE 256

#define NUM_ITERS 20

#define EPOCHS 3
// Global thread vars

int done = 0;                       // Number of finished tasks
double *theta;                  // Shared memory for theta
pthread_mutex_t lock, lock2;

struct args {
  Classifier* logReg;
  Hashtable* hashtable;
  Vocabulary* vocabulary;
  char** array;
  int start;
  int end;
};

void batch_train(void* params) {
  Classifier *logReg = (Classifier*)((struct args*)params)->logReg;
  Hashtable *hashtable = (Hashtable*)((struct args*)params)->hashtable;
  Vocabulary *vocabulary = (Vocabulary*)((struct args*)params)->vocabulary;
  char **array = (char**)((struct args*)params)->array;
  int start = ((struct args*)params)->start;
  int end = ((struct args*)params)->end;

  //printf("%d, %d, %d, %d\n", done, logReg->size, start, end);
  //printf("In thread %s\n", array[1]);

  int bowSize = (logReg->size-1)/2;
  // Create bath of X and Y
  double **bx = createX(array,start,end, hashtable, vocabulary,bowSize);
  //printf("createX OK\n");
  int *by = createY(array,start,end);
  //printf("createY OK\n");

  // Calculate theta
  double *tmp_theta = logisticRegression(logReg, bx, by, NUM_ITERS, BATCHSIZE);
  //printf("logRes OK\n");

  // Write W
  pthread_mutex_lock(&lock);
	for (int i=0; i<logReg->size; i++) {
    theta[i] += tmp_theta[i];
    //printf("%lf, ", theta[i]);
  }
  done++;
  printf("Done Tasks %d\n", done);
	pthread_mutex_unlock(&lock);
}

int main(int argc, char* argv[]){

/*
-----STARTING SECTION---------------
Start Program
Initialize input variables and data structures

*/

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



  char** stopwords= createStopWordsTable();
  ListNode* specsList=NULL;
  Hashtable* cliques = createHashtable(HASHTABLE_SIZE, BUCKET_SIZE);
  Vocabulary* vocabulary = createVocabulary(VOCABULARY_TABLE_SIZE, VOCABULARY_BUCKETSIZE);


/*
-----JSON SECTION---------------
Parse the json files into specs. then clean them up and get each on a single clique. Create
the vocabulary, as well as the shrunk array of words that we are going to use for training later.
*/

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

/*
-----CSV SECTION---------------
Parse the csv to adjust the cliques and create the three sets:
trainingSet, validationSet and testingSet.
*/

  printf("\nGoing through %s...\n",inputFile);

//open the input file for reading and a temporary set to save the final pairs
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

  //run the csv file and adjust the cliques

  printf("Adjusting cliques...\n");
  while(getline(&line, &len, fp) != -1){
    parseCsv(line, cliques, vocabulary, bowSize, logReg);

  }
  fclose(fp);


  //print all pairings into a file
  //our fix to not printing negatives more than once did not work, so we solved this through the linux file system

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

  //sort the file and keep the unique lines into a new file using a system command.

  char command[100];

  system("sort tempSet.csv | uniq > finalSet.csv");

  //open the file with all the pairings. this is the final set

  FILE *fp3;
  fp3= fopen("finalSet.csv", "r");
  if(fp3 == NULL){
    perror("Unable to open file!");
    exit(1);
  }

  line=NULL;
  len=0;

  //get the size

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

  //create the partitions
  fclose(fp3);
  fp3= fopen("finalSet.csv", "r");
  if(fp3 == NULL){
    perror("Unable to open file!");
    exit(1);
  }

  int c=0;
  line = NULL;
  len = 0;
  printf("FinalSize = %d, TrainingSize = %d, ValidationSize = %d, TestingSize = %d\n", finalSize, trainingSize, validationSize, testingSize);
  while(getline(&line, &len, fp3) != -1){
    line[strlen(line)-1]='\0';
    if(c<trainingSize){
      trainingSet[c]=strdup(line);
      //printf("ts %d : %s\n", c, trainingSet[c]);
    }
    else if(c>=trainingSize && c<(trainingSize+validationSize)){
      validationSet[c-trainingSize]=strdup(line);
      //printf("vs %d : %s\n", c, validationSet[c]);
    }
    else{
      testingSet[c-(trainingSize+validationSize)]=strdup(line);
      //printf("tests %d : %s\n", c, testingSet[c]);
    }
    c++;
}
  printf("Finised\n");

  shuffleArray(validationSet,validationSize);
  shuffleArray(testingSet,testingSize);
  fclose(fp3);

/*
-----TRAINING SECTION---------------
Train the model in a number of epochs
using multiple threads
*/
  threadpool tp;
  theta=(double*)malloc(sizeof(double)*logReg->size);

  printf("\nTraining the model...\n");
  if (threadpool_create(&tp, THREAD_NUM, QUEUE_SIZE) == 0)
    printf("Threadpool created\n");

  for(int j=0;j<EPOCHS;j++){
    printf("\n\n---------------------------\nEPOCH %d\n\n",j+1);
    // Add tasks
    for(int i=0; i<logReg->size; i++){
      theta[i]=0;
    }
    shuffleArray(trainingSet,trainingSize);
    int code, tasks = trainingSize/BATCHSIZE;
    struct args **params = (struct args**)malloc(sizeof(struct args*) * tasks);
    printf("Trying to add %d tasks\n", tasks);
    int start = 0, end = 0;
    for(int i=0; i<tasks;i++) {
      // Calculate batch range
      start = i * BATCHSIZE;
      end = start + BATCHSIZE;
      end = (end > trainingSize) ? trainingSize : end;
      // Create parameter struct
      params[i]= (struct args*)malloc(sizeof(struct args));
      params[i]->logReg = logReg;
      params[i]->vocabulary = vocabulary;
      params[i]->hashtable = cliques;
      params[i]->array = trainingSet;
      params[i]->start = start;
      params[i]->end = end;

      printf("Adding task %d, range (%d - %d)...\n", i+1, params[i]->start, params[i]->end);
      // Wait for emty spot in queue and add task
      while( (code = threadpool_add(tp, &batch_train, params[i])) == -5) {
        printf("Retrying to add task %d...\n", i);
      }

      if (code != 0) {
        fprintf(stderr, "Error on %d\n", i);
        return code;
      }
      printf("Added %d\n", i+1);
    }

    //Wait all task to finish
    while (done < tasks) {
      usleep(10);
    }

    // Calculate average W
    printf("Before\n\n");
    for (int i=0; i<logReg->size; i++) {
      printf("%lf, ", theta[i]);
      theta[i] /= tasks;


    }

    printf("Now after\n\n");
    for (int i=0; i<logReg->size; i++) {
      logReg->w[i]=theta[i];
      printf("%lf, ", logReg->w[i]);

    }

    for (int i=0; i<tasks; i++) {
      free(params[i]);
    }
    free(params);
  }


  if (threadpool_exit(tp) == 0) printf("Thread pool deleted\n");
/*
-----VALIDATION SECTION---------------
Use the validation set to resolve conflicts, until their Number
is minimized
*/
  printf("\nValidating and resolving conflicts...\n");
  int totalConflicts=validate(validationSet, validationSize, logReg, cliques, vocabulary);
  printf("size: %d totalConflicts: %d\n",validationSize,totalConflicts);

/*
-----TESTING SECTION---------------
Use the testing set to test the predictions that the model is going to
produce.
*/


/*
-----FINAL SECTION---------------
Finish Program
Print statistics
Free memory
*/

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
  free(theta);
  free(inputFile);
  free(line);

  deleteVocabulary(vocabulary);

  deleteHashtable(cliques);
  deleteList(specsList);
  deleteClassifier(logReg);

  fclose(fp4);

  return 0;


}
