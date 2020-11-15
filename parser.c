#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "specs.h"
#include "lowlevel-io.h"

#define DATAPATH "Datasets/2013_camera_specs"

Specs* parser(char* directory, char* file){

	FILE *fp;
	char *key, *temp_value, *value;

	char* dirpath = createPath(DATAPATH, directory);
	char* filepath =createPath(dirpath, file);
	deletePath(dirpath);

	if ((fp = fopen(filepath,"r")) == NULL){
		perror("open of file failed");
		exit(EXIT_FAILURE);
	}
	deletePath(filepath);

	int quotation_mark = 0, key_size = 0, value_size = 0, current_character;

	if ((key = malloc(sizeof(char))) == NULL){
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	if ((temp_value = malloc(sizeof(char))) == NULL){
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}

	char* id = createSpecsID(directory, file);
	Specs* spec = createSpecs(id);

	while ((current_character = fgetc(fp)) != EOF){

		// when we change line if there was a quotation mark in the previous line, then that line contains a spec key and a corresponding value
		// if there was no quotation mark in the previous line, that line must be ignored, because it doesn't contain useful information

		if ((char)current_character == '\n'){

			if (quotation_mark > 0){

				quotation_mark = 0;

				key_size++;
				if ((key = realloc(key, key_size * sizeof(char))) == NULL){
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}
				key[key_size - 1] = '\0';

				value_size++;
				if ((temp_value = realloc(temp_value, value_size * sizeof(char))) == NULL){
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}
				temp_value[value_size - 1] = '\0';
				value_size--;

				while (temp_value[value_size] != '"')
					value_size--;

				if ((value = malloc((value_size + 1) * sizeof(char))) == NULL){
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}

				memcpy(value, temp_value, value_size);

				value[value_size] = '\0';


				//printf("key: %s\nvalue: %s\n\n", key, value_size, (int)strlen(value), temp_value, value);
				insertSpecs(&spec, key, value);
				free(key);
				free(temp_value);
				free(value);
				key_size = 0;
				value_size = 0;
				if((key = malloc (sizeof(char))) == NULL){
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}

				if((temp_value = malloc (sizeof(char))) == NULL){
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}
			}
		}


		// when the current charachter is a quotation mark, the corresponding variable is incremented by 1

		if ((char)current_character == '"')
			quotation_mark++;


		// when the variable quotation_mark equals 2, we start "building" the key string

		if (quotation_mark == 2){
			key_size++;
			if((key = realloc(key, key_size * sizeof(char))) == NULL){
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			key[key_size-1] = (char) current_character;
		}


		// when the variable quotation_mark equals 5, we start "building" the value string

		if (quotation_mark >= 5){
			value_size++;

			if ((temp_value = realloc(temp_value, value_size * sizeof(char))) == NULL){
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}

			temp_value[value_size - 1] = (char) current_character;
		}


		/* when the quotation mark is found for the 1st time in the line, the variable quotation_mark equals 1
		   when the quotation mark is found for the 3rd time in the line, the variable quotation_mark equals 4

		   in both cases, we increment the variable quotation mark by one
		   we do so, so that we bypass the quotation mark and start "building" our key or our value string accordingly from the next character */

		if (quotation_mark == 1 || quotation_mark == 4)
			quotation_mark++;

	}
	if(fclose(fp)==EOF){
		perror("Cannot close file");
	}

	deleteSpecsId(id);
	return spec;

}
