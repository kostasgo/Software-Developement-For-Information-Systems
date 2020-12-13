#ifndef bucket_h
#define bucket_h

#include "clique.h"
typedef struct BucketType{
  /*
  This is the struct of the bucket. It contains a single clique, and has a pointer
  to the next bucket
  */
  Clique** cliques;
  struct BucketType* next;
  int max;
}Bucket;

Bucket* createBucket(int);
void insertBucket(Bucket**, Specs*);
void deleteBucket(Bucket*);
Clique* searchBucket(char*, Bucket*);
void printBucket(Bucket*);


#endif
