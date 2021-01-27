#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "hashtable.h"
#include "clique.h"
#include "negative-cliques.h"
#include "bow.h"
#include "words.h"
#include "logistic_regression.h"
#include "bst.h"

#define NUM_ITERS 10
#define THRESHHOLD 0.05
#define PRED_THRESH 0.3

void swapStrings(char** str1, char** str2){
  char* temp =*str1;
  *str1 = *str2;
  *str2 = temp;
}

void shuffleArray(char** arr, size_t n){
  /*
  Shuffles an array of strings
  */
  if(n>1){
    size_t i;
    for(i=0;i<n-1;++i){
      size_t rnd = (size_t)rand();
      size_t j =( i + rnd) % (n - i);

      swapStrings(&arr[i],&arr[j]);
    }
  }
}

void mergeCliques(Hashtable* table, char* id1, char* id2){
  /*
  Takes two files that should be in the same clique and updates
  their clique, as well as every cliquemember's clique pointer,
  so that they all point to the same clique.
  */

  BucketData* data1=searchHashtable(table, id1);
	BucketData* data2=searchHashtable(table, id2);


  if(data1->clique==NULL){
		printf("%s not in hashtable\n",id1);
		return;
	}
	if(data2->clique==NULL){
		printf("%s not in hashtable\n",id2);
		return;
	}
  BucketData *new, *old;

  if(compareCliques(data1->clique,data2->clique)){return;}

  //printf("sizes before: %d %d\n", data1->clique->size, data2->clique->size);
  //printClique(data1->clique);
  //printClique(data2->clique);
  if(data1->clique->size>=data2->clique->size){
    new = data1;
    old = data2;
  }
  else{
    new = data2;
    old = data1;
  }

  Clique * temp=old->clique;
  concatCliqueList(new->clique->list,temp->list);
  new->clique->size=temp->size + new->clique->size;

  NegListNode* negs=old->clique->negatives;
  while(negs!=NULL){
    insertNegatives(&(new->clique->negatives), negs->clique);
    negs=negs->next;
  }


  CliqueNode* node = old->clique->list;
  //printClique(old->clique);
  //printf("Size is: %d\n\n",old->clique->size);
  int size = old->clique->size;
  while(node!=NULL){
    //printf("%d\n",i);

    BucketData* temp_data = searchHashtable(table, node->specs->id);
    //deleteClique(temp_data->clique);
    temp_data->clique=new->clique;
    temp_data->flag=1;
    node=node->next;

  }
  data1=searchHashtable(table, id1);
  data2=searchHashtable(table, id2);
  //printf("sizes after: %d %d\n", data1->clique->size, data2->clique->size);
  //printClique(data1->clique);
  //printClique(data2->clique);

}

void updateNegatives(Hashtable* table, char* id1, char* id2){
  /*
  Takes two files that are negative to each other and updates the Cliques
  they are in, so that they are marked as negative to each other.
  */
  BucketData* data1=searchHashtable(table, id1);
	BucketData* data2=searchHashtable(table, id2);

  if(data1->clique==NULL){
		printf("%s not in hashtable\n",id1);
		return;
	}
	if(data2->clique==NULL){
		printf("%s not in hashtable\n",id2);
		return;
	}
  insertNegatives(&(data1->clique->negatives), data2->clique);
  insertNegatives(&(data2->clique->negatives), data1->clique);
}

double* getTfIdfArray(Hashtable* table, char* id, Vocabulary* vocabulary, int bowSize){

  //create an array of doubles and fill it with 0
  double* array=(double*)malloc(sizeof(double)*bowSize);
  for(int i=0; i<bowSize; i++){
    array[i]=0;
  }

  //search the hashtable for the specific file
  BucketData* data=searchHashtable(table, id);
  if(data->clique==NULL){
		printf("%s not in hashtable\n",id);
		return array;
	}

	CliqueNode* temp=data->clique->list;
  Specs* specs;

  while(temp!=NULL){
    if(!strcmp(temp->specs->id,id)){
      specs=temp->specs;
      break;
    }
    temp=temp->next;
  }
  //go through the words in the file and see if they are in the bow.
  //if they are update the tf-idf in the array
  CorrectNode* current=specs->words;
  while(current!=NULL){
    Word* word=searchVocabulary(vocabulary, current->word);
    //word = searchVocabulary(vocabulary, current->word);
    if(word->index!=-1){
      array[word->index]=current->counter;

    }
    current=current->next;
  }

  return array;
}


double** createX(char** array, int start, int end, Hashtable* table, Vocabulary* vocabulary, int bowSize){
  //define batchSize
  int batchSize= end - start;
  //printf("Batch size in createX = %d\n", batchSize);

  //allocate memory for x
  double **x= (double**)malloc(sizeof(double*)*batchSize);

  for(int i=start; i<end; i++){
    //for each line in batch size
    char* line;
    char *savep1;
    //printf("%s\n", array[i]);
    line=strdup(array[i]);

    char* spec_1, *spec_2, *temp;

  	char delim[2] =",";

  	char *token = strtok_r(line, delim, &savep1);
  	spec_1=strdup(token);

    //printf("After strtok1 %s\n", array[i]);

  	token = strtok_r(NULL, delim, &savep1);
    spec_2 = strdup(token);

    //printf("After strtok2 %s\n", array[i]);

    //get the tf-idf array for each spec

    double* array1= getTfIdfArray(table, spec_1, vocabulary, bowSize);
    double* array2= getTfIdfArray(table, spec_2, vocabulary, bowSize);

    //pass them into x[i]
    x[i-start] = (double*)malloc(sizeof(double)*(bowSize*2+1));
    for(int j=0; j<bowSize; j++){
      x[i-start][j]=array1[j];
      x[i-start][bowSize + j]=array2[j];
    }


    //last position of x[i] is used for the bias
    x[i-start][2*bowSize]=1;

    free(array1);
    free(array2);
    free(spec_1);
    free(spec_2);
    free(line);
  }
  return x;
}

int* createY(char** array, int start, int end){
  int batchSize= end - start;

  int* y=(int*)malloc(sizeof(int)*batchSize);

  for(int i=start; i<end;i++){
    char* spec_1, *spec_2, *temp;
  	int label;

    char* line, *savep1;
    line=strdup(array[i]);

  	char delim[2] =",";

  	char *token = strtok_r(line, delim, &savep1);
  	token = strtok_r(NULL, delim, &savep1);
  	token = strtok_r(NULL, delim, &savep1);

    label = strtol(token, &temp, 10);

    y[i-start]=label;
    free(line);
  }
  return y;
}

void parseCsv(char* line, Hashtable* table, Vocabulary* vocabulary, int bowSize, Classifier* logReg){
	/*
	This function reads a .csv line. Finds the two keys in the hashtable,
	and merges their cliques if they are in different cliques, otherwise it inserts
  both cliques to each other's negatives lists.
	*/

	//printf("%s\n",line);
	char* spec_1, *spec_2, *temp;
	int label, pos_1, pos_2;

	char delim[3] =", ";

	char *token = strtok(line, delim);
	spec_1=strdup(token);

	token = strtok(NULL, delim);
  spec_2 = strdup(token);

	token = strtok(NULL, delim);
  label = strtol(token, &temp, 10);

	if(label==0){
		updateNegatives(table, spec_1, spec_2);
	}
  else{
    mergeCliques(table, spec_1, spec_2);


  }
/*

  //create x
  double* array1= getTfIdfArray(table, spec_1, vocabulary, bowSize);
  double* array2= getTfIdfArray(table, spec_2, vocabulary, bowSize);
  double* x = (double*)malloc(sizeof(double)*(bowSize*2+1));
  for(int i=0; i<bowSize; i++){
    x[i]=array1[i];
    x[bowSize + i]=array2[i];
  }

  x[2*bowSize]=1;

  if(counter<lines){
    //train the model
    logisticRegression(logReg, x, label, NUM_ITERS);
    //logisticRegression(logReg, x, label, NUM_ITERS);
    if(label==1){ //Train the model extra for 1s to compensate the bias
      for(int i=0; i<4; i++){
        logisticRegression(logReg, x, label, NUM_ITERS);
      }

    }
  }
  else{
    //predict an output
    //BucketData* data1=searchHashtable(table, spec_1);
  	//BucketData* data2=searchHashtable(table, spec_2);
    double h = hypothesis(logReg->w,x,logReg->size);
    int prediction;
    if(h>=0.5){
      prediction=1;

    }
    else{
      prediction=0;

    }
    fprintf(fp,"%s,%s,%d\n",spec_1,spec_2,prediction);
  }
  free(array1);
  free(array2);
  free(x);
*/
  free(spec_1);
  free(spec_2);


	//printf("Merging...pos_1: %d pos_2: %d\n", pos_1, pos_2);
}

int validate(char** array, int size, Classifier* logReg, Hashtable* table, Vocabulary* vocabulary){

    printf("Validation: size: %d\n",size);
    int totalPredictions=0;
    int totalPredictionsKept=0;
    int totalKept=0;
    ListNode* speclist=NULL;
    Hashtable* temp_cliques = createHashtable(4000, 4);

    //create an x[] for each line of the array
    int bowSize = (((logReg->size)-1)/2);
    double** x=createX(array, 0, size, table, vocabulary, bowSize);

    TreeNode* bst=NULL;

    for(int i=0; i<size; i++){

      double h = hypothesis(logReg->w,x[i],logReg->size);//make a prediction for given row

      //test if prediction was below 0.5 and adjust the score and prediction accordingly
      double score;
      int prediction;
      if (h<PRED_THRESH){
        score=(PRED_THRESH-h);
        prediction=0;
      }
      else{
        score=h-PRED_THRESH;
        double bonus=(0.5-PRED_THRESH)/5;
        score=score+bonus;
        prediction=1;
      }
      //if score below threshhold, ignore it (set it to 0)
      //printf("%s\n",array[i]);
      for(int j=0; j<logReg->size;j++){
        //printf("%lf, ",x[i][j]);
      }


      if(score<THRESHHOLD){
        score=(double)0;
      }

      //make score to be from 0 to 10
      score*= (double)20;
      //printf("hypothesis: %lf score: %lf prediction: %d\n\n",h,score,prediction);
      bst=insertTree(bst, score, i, prediction, x[i]);
      if(prediction==1) totalPredictions++;
      if(score!=0) totalKept++;
      if(score!=0 && prediction==1) totalPredictionsKept++;
    }
    printf("total ones: %d\n",totalPredictions);
    printf("total ones kept: %d\n",totalPredictionsKept);
    printf("total kept: %d\n",totalKept);

    int totalConflicts= inOrderValidation(bst, array, temp_cliques, logReg, &speclist);
    for(int i=0; i<size; i++){
      free(x[i]);
    }

    free(x);
    deleteTree(bst);
    deleteList(speclist);
    deleteHashtable(temp_cliques);

    return totalConflicts;
}
