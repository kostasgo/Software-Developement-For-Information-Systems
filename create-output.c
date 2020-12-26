#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"
#include "clique.h"
#include "negative-cliques.h"

void mergeCliques(Hashtable* table, char* id1, char* id2){

  BucketData* data1=searchHashtable(table, id1);
	BucketData* data2=searchHashtable(table, id2);


  if(data1->clique==NULL){
	//	printf("%s not in hashtable\n",id1);
		return;
	}
	if(data2->clique==NULL){
		//printf("%s not in hashtable\n",id2);
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

void parseCsv(char* line, Hashtable* table){
	/*
	This function reads a .csv line, finds the two keys in the hashtable,
	and merges their cliques if they are in different cliques.
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
		(table, spec_1, spec_2);
		free(spec_1);
		free(spec_2);
		return;
	}



	//printf("Merging...pos_1: %d pos_2: %d\n", pos_1, pos_2);
	mergeCliques(table, spec_1, spec_2);
	free(spec_1);
	free(spec_2);

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
