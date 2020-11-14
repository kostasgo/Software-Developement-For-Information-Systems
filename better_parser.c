#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(){
	
	
	
	FILE *fp;
	char *key, *temp_value, *value;
	fp = fopen("4233.json","r");
	char vrethike_eisagwgiko = 0;
	int key_size = 0, value_size = 0;
	int trexon_xaraktiras;
	key = malloc (sizeof(char));
	temp_value = malloc (sizeof(char));
	value = malloc (sizeof(char));
	
	while ((trexon_xaraktiras = fgetc(fp)) != EOF){
		
		if ((char)trexon_xaraktiras == '\n'){
			if(vrethike_eisagwgiko > 0){
				vrethike_eisagwgiko = 0;
				key_size++;
				key = realloc(key, key_size * sizeof(char));
				key[key_size - 1] = '\0';
				value_size++;
				value = realloc(value, value_size * sizeof(char));
				temp_value[value_size - 1] = '\0';
				value_size--;
				
				while(temp_value[value_size] != '"')
					value_size--;
					
				value = realloc (value, (value_size + 1) * sizeof(char));
				
				memcpy(value, temp_value, value_size - 1);
				
				value[value_size] = '\0';
					
				
				
				printf("key: %s\nvalue: %s\n\n\n", key, value);
				free(key);
				free(temp_value);
				free(value);
				key_size = 0;
				value_size = 0;
				key = malloc (sizeof(char));
				temp_value = malloc (sizeof(char));
				value = malloc (sizeof(char));
			}
		}
		
		if ((char)trexon_xaraktiras == '"')
			vrethike_eisagwgiko++;
		
		if (vrethike_eisagwgiko == 2){
			key_size++;
			key = realloc(key, key_size * sizeof(char));
			key[key_size-1] = (char) trexon_xaraktiras;
		}
		
		
		if (vrethike_eisagwgiko >= 5){
			value_size++;
			temp_value = realloc(temp_value, value_size * sizeof(char));
			temp_value[value_size - 1] = (char) trexon_xaraktiras;
		}
		
		if (vrethike_eisagwgiko == 1 || vrethike_eisagwgiko == 4)
			vrethike_eisagwgiko++;
		
	}
	
}
