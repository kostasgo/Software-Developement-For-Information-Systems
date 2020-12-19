
#ifndef bucket_h
#define bucket_h

#include "clique.h"

typedef struct BucketDataType{
  char* id;
  Clique* clique;
  int flag;
}BucketData;

typedef struct BucketType{

  int max;
  BucketData** data;
  struct BucketType* next;

}Bucket;

BucketData* createBucketData(char *,Clique*);
void deleteBucketData(BucketData*);
Bucket* createBucket(int);

BucketData* searchBucket(Bucket *, char*);
//void bucketInsert(Bucket**, BucketData*);
void printBucket(Bucket*);
void deleteBucket(Bucket*);

#endif
