#include "sparse_vectors.h"
#include <stdlib.h>
#include <stdio.h>

SparseV* createSparseV(double* x, int xSize, int size){
  SparseV* vec=(SparseV*)malloc(sizeof(SparseV));
  vec->data= (double*)malloc(sizeof(double)*size);
  vec->index = (int*)malloc(sizeof(int)*size);
  vec->size=size;
  int counter=0;
  for(int i=0;i<xSize;i++){
    if(x[i]!=0){

      vec->data[counter]=x[i];
      vec->index[counter]=i;
      counter++;
    }
    if(counter>size){
      break;
    }
  }

  return vec;
}


void deleteSparseV(SparseV* v){
  free(v->data);
  free(v->index);
  free(v);
  v=NULL;
}


SparseV* concatSparseV(SparseV* v1, SparseV* v2, int totalSize){
  SparseV* result=(SparseV*)malloc(sizeof(SparseV));
  result->size=v1->size+v2->size;
  result->data=(double*)malloc(sizeof(double)*result->size);
  result->index=(int*)malloc(sizeof(int)*result->size);

  for(int i=0; i<v1->size; i++){
    result->data[i]=v1->data[i];
    result->index[i]=v1->index[i];
  }
  for(int i=v1->size; i<result->size; i++){
    result->data[i]=v2->data[i-v1->size];
    result->index[i]=v2->index[i-v1->size] + totalSize;
  }

  deleteSparseV(v1);
  deleteSparseV(v2);

  return result;
}

double getPosValue(SparseV* x, int pos){
  double ret=0;
  for(int i=0; i<x->size; i++){
    if(x->index[i]==pos){
      ret=x->data[i];
      break;
    }
  }
  return ret;
}

void printSparseV(SparseV* x){
  printf("size: %d\n|",x->size);
  for(int i=0; i<x->size; i++){
    printf("%lf|",x->data[i]);
  }
  printf("\n|");
  for(int i=0; i<x->size; i++){
    printf("%d|",x->index[i]);
  }
  printf("\n");
}
