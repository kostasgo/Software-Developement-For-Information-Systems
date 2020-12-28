#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "clique.h"
#include "negative-cliques.h"
#include "bow.h"
#include "words.h"

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



  CliqueNode* node = old->clique->list;
  //printClique(old->clique);
  //printf("Size is: %d\n\n",old->clique->size);
  int size = old->clique->size;
  for(int i=0; i<size; i++){
    //printf("%d\n",i);
    int pos_temp=-1;
    BucketData* temp_data = searchHashtable(table, node->specs->id);
    //deleteClique(temp_data->clique);
    temp_data->clique=new->clique;
    temp_data->flag=1;
    node=node->next;

  }

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
      array[word->index]=word->idf*current->tf;

    }
    current=current->next;
  }

  return array;
}

void parseCsv(char* line, Hashtable* table, Vocabulary* vocabulary, Word** bow, int bowSize){
	/*
	This function reads a .csv line. Finds the two keys in the hashtable,
	and merges their cliques if they are in different cliques, otherwise it inserts
  both cliques to each other's negatives lists. Afterwards it creates an
  array of tf-idf for each spec, that will be used in the logistic regressor.
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

  double* array1= getTfIdfArray(table, spec_1, vocabulary, bowSize);
  double* array2= getTfIdfArray(table, spec_2, vocabulary, bowSize);

  free(spec_1);
  free(spec_2);


	//printf("Merging...pos_1: %d pos_2: %d\n", pos_1, pos_2);


}

void outputToFile(Hashtable* table){
	/*
	Does the final output. Runs through the whole hashtable and
	for for each clique that has at least two members, it prints every two
	of its members in the accepted .csv format
	*/
	FILE *fp;
  fp= fopen("output.csv", "w");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }

  fprintf(fp,"left_spec_id, right_spec_id\n");
	for(int i=0; i<table->max;i++){
		Bucket* temp= table->array[i];
		while(temp!=NULL){
			for(int j=0; j<temp->max; j++){
				if(temp->data[j]->clique==NULL){
					continue;
				}
				if(temp->data[j]->flag==1){
					continue;
				}
				if(temp->data[j]->clique->size>1){

					CliqueNode* head = temp->data[j]->clique->list;
					CliqueNode* temp1 = head;
					CliqueNode* temp2 = temp1;
					//printf("%d\n", table->array[i]->size);
				//	printClique(temp->array[j]);
					while(temp1!=NULL){
						temp2=temp2->next;
						while(temp2!=NULL){
							//printf("%s, %s\n",temp1->specs->id, temp2->specs->id);
							fprintf(fp,"%s, %s\n",temp1->specs->id, temp2->specs->id);
							temp2=temp2->next;
						}

						temp1=temp1->next;
						temp2=temp1;
					}
				}
			}

			temp=temp->next;
		}

	}
	fclose(fp);
}
