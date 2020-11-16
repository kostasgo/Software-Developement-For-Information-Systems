#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void adjustCliques(char* line, Hashtable** table){
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

	if(pos_1<pos_2){
		mergeCliques((*table)->array[pos_1], (*table)->array[pos_2]);
	}
	else{
		mergeCliques((*table)->array[pos_2], (*table)->array[pos_1]);
	}
}
