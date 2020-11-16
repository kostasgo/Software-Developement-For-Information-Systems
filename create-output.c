#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void adjustCliques(char* line, Hashtable** table){
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
		return;
	}

	pos_1=searchHashtable(*table, spec_1);
	pos_2=searchHashtable(*table, spec_2);


	//printf("Merging...pos_1: %d pos_2: %d\n", pos_1, pos_2);

	if(pos_1<pos_2){
		mergeCliques((*table)->array[pos_1], (*table)->array[pos_2]);
	}
	else if(pos_1>pos_2){
		mergeCliques((*table)->array[pos_2], (*table)->array[pos_1]);
	}
	else{
		return;
	}
}

void outputToFile(Hashtable* table){
	FILE *fp;
  fp= fopen("output.csv", "w");
  if(fp == NULL){
    perror("Unable to open file!");
    exit(1);
  }

  fprintf(fp,"left_spec_id, right_spec_id\n");
	for(int i=0; i<table->max;i++){
		if(table->array[i]->size>1){

			CliqueNode* head = table->array[i]->list;
			CliqueNode* temp1 = head;
			CliqueNode* temp2 = temp1;
			//printf("%d\n", table->array[i]->size);
			printClique(table->array[i]);
			while(temp1!=NULL){
				temp2=temp2->next;
				while(temp2!=NULL){
					printf("%s, %s\n",temp1->specs->id, temp2->specs->id);
					fprintf(fp,"%s, %s\n",temp1->specs->id, temp2->specs->id);
					temp2=temp2->next;
				}

				temp1=temp1->next;
				temp2=temp1;
			}
		}
	}
	fclose(fp);
}
