#ifndef word_h
#define word_h

typedef struct WordBucketDataType{
  char* id;
  Clique* clique;
  int flag;
}WordBucketData;

typedef struct WordBucketType{

  int max;
  WordBucketData** data;
  struct WordBucketType* next;

}WordBucket;

WordBucketData* createWordBucketData(char *,Clique*);
void deleteWordBucketData(WordBucketData*);
WordBucket* createWordBucket(int);

WordBucketData* searchWordBucket(WordBucket *, char*);
//void bucketInsert(Bucket**, BucketData*);
void printWordBucket(WordBucket*);
void deleteWordBucket(WordBucket*);

#endif
