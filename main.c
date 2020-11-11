#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "specs.h"

int main(int argc, char* argv[]){

  char* dir= strdup("www.ebay.com");
  char* f= strdup("32000.json");
  char* spec;
  spec=createSpecsID(dir,f);

  printf("%s\n",spec);
  free(spec);

  return 0;
}
