#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
	
	FILE *fp;
	char *key, *temp_value, *value;
	fp = fopen("4233.json","r");
	int quotation_mark = 0;
	int key_size = 0, value_size = 0;
	int current_character;
	key = malloc (sizeof(char));
	temp_value = malloc (sizeof(char));
	
	while ((current_character = fgetc(fp)) != EOF){
		
		// when we change line if there was a quotation mark in the previous line, then that line contains a spec key and a corresponding line
		// if there was no quotation mark in the previous line, that line must be ignored, because it doesn't contain useful information
		
		if ((char)current_character == '\n'){
			
			if(quotation_mark > 0){
			
				quotation_mark = 0;
			
				key_size++;
				key = realloc(key, key_size * sizeof(char));
				key[key_size - 1] = '\0';
			
				value_size++;
				temp_value[value_size - 1] = '\0';
				value_size--;
				
				while(temp_value[value_size] != '"')
					value_size--;
					
				value = malloc ((value_size + 1) * sizeof(char));
				
				memcpy(value, temp_value, value_size);
				
				value[value_size] = '\0';
					
				
				
				//printf("key: %s\nvalue size: %d\nvalue string length: %d\ntempvalue: %s\nvalue: %s\n\n\n", key, value_size, (int)strlen(value), temp_value, value);
				printf("key: %s\ntemp value: %s\nvalue: %s\n\n", key, temp_value, value);
				free(key);
				free(temp_value);
				free(value);
				key_size = 0;
				value_size = 0;
				key = malloc (sizeof(char));
				temp_value = malloc (sizeof(char));
			}
		}
		
		
		// when the current charachter is a quotation mark, the corresponding variable is incremented by 1
		
		if ((char)current_character == '"')
			quotation_mark++;
			
		
		// when the variable quotation_mark equals 2, we start "building" the key string
		
		if (quotation_mark == 2){
			key_size++;
			key = realloc(key, key_size * sizeof(char));
			key[key_size-1] = (char) current_character;
		}
		
		
		// when the variable quotation_mark equals 5, we start "building" the value string
		
		if (quotation_mark >= 5){
			value_size++;
			temp_value = realloc(temp_value, value_size * sizeof(char));
			temp_value[value_size - 1] = (char) current_character;
		}
		
		
		/* when the quotation mark is found for the 1st time in the line, the variable quotation_mark equals 1
		   when the quotation mark is found for the 3rd time in the line, the variable quotation_mark equals 4
		   
		   in both cases, we increment the variable quotation mark by one
		   we do so, so that we bypass the quotation mark and start building our key or our value string accordingly from the next character */
		
		if (quotation_mark == 1 || quotation_mark == 4)
			quotation_mark++;
		
	}
	
}
