#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "specs.h"
#include "lowlevel-io.h"
#include "keyvalue.h"

#define DATAPATH "Datasets/2013_camera_specs"

Specs* parser(char* directory, char* file){

	/*
	This function looks at given .json file in given directory and parses
	its data into a Specs struct. Then returns a pointer to the Struct.
	*/

	FILE *fp;
	char *key, *temp_value, *value, *value2;

	char* dirpath = createPath(DATAPATH, directory);
	char* filepath = createPath(dirpath, file);
	deletePath(dirpath);

	if ((fp = fopen(filepath,"r")) == NULL){
		perror("Open of file failed");
		exit(EXIT_FAILURE);
	}
	deletePath(filepath);

	int quotation_mark = 0, key_size = 0, value_size = 0, current_character, bracket = 0;
//	char former_character;

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

	Value *val = NULL;
	KV_Pair *pair;


	while ((current_character = fgetc(fp)) != EOF){

		// when the current charachter is a quotation mark, the corresponding variable is incremented by 1

		if ((char)current_character == '"'){
			quotation_mark++;
			//printf("found qm!\n");
		}



		/* when the variable quotation_mark equals 2, we start "building" the key string
		   the loop keeps running as long as quotation_mark equals 2
		   which means until we find another quotation mark
		*/
		//printf("QM: %d, CC: %c\n", quotation_mark, (char)current_character);

		if (quotation_mark == 3 && (char)current_character=='"'){
			//printf("in!\n");
			key_size++;
			if ((key = realloc(key, key_size * sizeof(char))) == NULL){
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			key[key_size - 1] = '\0';

			//quotation_mark++;

		}

		while (quotation_mark == 2){

			// the current character read from the file is inserted in the key string
			key_size++;
			if((key = realloc(key, key_size * sizeof(char))) == NULL){
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			key[key_size-1] = (char) current_character;


			/* when we find a quotation mark the key string must be finalised
			   and value 3 must be assigned to the quotation_mark variable
			   so that the loop ends and the rest function continues
			 */

			current_character = fgetc(fp);

			if((char)current_character == '"'){

				key_size++;
				if ((key = realloc(key, key_size * sizeof(char))) == NULL){
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}
				key[key_size - 1] = '\0';

				//value2 = strdup(key);
				//printf("value2 = %s\n", value2);

				quotation_mark++;

			}

		}


		/* when we search for a value and a left square bracket is found
		   before a quotation mark, that means that we have multiple values for that key
		   so we assign the bracket variable the value 1
		   so that we insert the corresponding loop
		*/

		if (quotation_mark == 3 && (char)current_character == '[')
			bracket = 1;

		// when the variable quotation_mark equals 5, we start "building" the value string


		while (bracket){

			/* ignore every character until a quotation mark or a right square bracket is found
				the first means that the line has a value
				the second that there are no more values for the current key
			*/

			do{
				current_character = fgetc(fp);
			}while ((char)current_character != '"' && (char)current_character != ']');


			// if a quotation mark is found first then a value string must begin to be built

			if ((char)current_character == '"'){

				current_character = fgetc(fp);

				// as long as no change line is found, we keep building the value string

				do{
					value_size++;

					if ((temp_value = realloc(temp_value, value_size * sizeof(char))) == NULL){
						perror("realloc failed");
						exit(EXIT_FAILURE);
					}

					temp_value[value_size - 1] = (char) current_character;

					current_character = fgetc(fp);

				}while ((char)current_character != '\n');
			}

			/* if the current character is a change of line
			   it's time to finalise the value string and insert it in Specs
			*/

			if((char)current_character == '\n'){

				value_size++;
				if ((temp_value = realloc(temp_value, value_size * sizeof(char))) == NULL){
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}
				temp_value[value_size - 1] = '\0';


				value_size--;

				while (temp_value[value_size] != '"')
					value_size--;

			//	printf("value size :%d\n", value_size);

				if ((value = malloc((value_size + 1) * sizeof(char))) == NULL){
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}

				memcpy(value, temp_value, value_size);

				value[value_size] = '\0';
				//printf("key: %s\nvalue: %s\n\n", key, value);

				//insert
				insertValue(&val, value);

				// prepare for the next value to be found
				free(value);
				free(temp_value);
				value_size = 0;

				if((temp_value = malloc (sizeof(char))) == NULL){
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}

			}

			/* if a right square bracket is found before a quotation mark in a line
			   that means that the current key has no more values
			   so we must prepare for the next pair
			*/

			if ((char)current_character == ']'){

				pair = createKV(key, val);
				insertSpecs(&spec, pair);
				//val = NULL;
				free(key);
				key_size = 0;
				value_size = 0;
				val=NULL;
				if((key = malloc (sizeof(char))) == NULL){
					perror("malloc failed");
					exit(EXIT_FAILURE);
				}
				bracket = 0;
				quotation_mark = 0;

			}

		}



		if (quotation_mark == 5){

			do {
				value_size++;

				if ((temp_value = realloc(temp_value, value_size * sizeof(char))) == NULL){
					perror("realloc failed");
					exit(EXIT_FAILURE);
				}

				temp_value[value_size - 1] = (char) current_character;

				current_character = fgetc(fp);

			}while ((char)current_character != '\n');

			value_size++;

			if ((temp_value = realloc(temp_value, value_size * sizeof(char))) == NULL){
				perror("realloc failed");
				exit(EXIT_FAILURE);
			}
			temp_value[value_size - 1] = '\0';


			value_size--;

			while (temp_value[value_size] != '"')
				value_size--;

		//	printf("value size :%d\n", value_size);

			if ((value = malloc((value_size + 2) * sizeof(char))) == NULL){
				perror("malloc failed");
				exit(EXIT_FAILURE);
			}

			memcpy(value, temp_value, value_size);

			value[value_size] = '\0';

			//insert
			insertValue(&val, value);
			pair = createKV(key, val);
			insertSpecs(&spec, pair);

			//val = NULL;
			free(key);
			free(temp_value);
			free(value);
			val=NULL;
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

			quotation_mark = 0;

		}


		if (quotation_mark == 1 || quotation_mark == 4){
		//	printf("Incrementing!\n");
			quotation_mark++;
		}



	//	former_character = (char) current_character;

	}


	if(fclose(fp)==EOF){
		perror("Cannot close file");
	}

	deleteSpecsId(id);

	return spec;

}
