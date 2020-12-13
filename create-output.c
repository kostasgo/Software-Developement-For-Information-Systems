#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void adjustCliques(char* line, Hashtable** table){
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
		free(spec_1);
		free(spec_2);
		return;
	}

	Clique* clique1=searchHashtable(*table, spec_1);
	Clique* clique2=searchHashtable(*table, spec_2);
	if(clique1==NULL){
		//printf("%s not in hashtable\n",spec_1);
		return;
	}
	if(clique2==NULL){
		//printf("%s not in hashtable\n",spec_2);
		return;
	}
	free(spec_1);
	free(spec_2);

	//printf("Merging...pos_1: %d pos_2: %d\n", pos_1, pos_2);

	if(compareCliques(clique1,clique2)){return;}

	//printf("Merging\n");

	mergeCliques(clique1, clique2);
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
				if(temp->cliques[j]->size>1){

					CliqueNode* head = temp->cliques[j]->list;
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
