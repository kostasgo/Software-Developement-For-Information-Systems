#ifndef word_h
#define word_h

typedef struct WordType{
  char* str;
  int counter;
}Word;

typedef struct VocBucketType{
  int max;
  Word** words;
  struct VocBucketType* next;
}VocBucket;


WordBucketData* createWordBucketData(char *,Clique*);
void deleteWordBucketData(WordBucketData*);
WordBucket* createWordBucket(int);

WordBucketData* searchWordBucket(WordBucket *, char*);
//void bucketInsert(Bucket**, BucketData*);
void printWordBucket(WordBucket*);
void deleteWordBucket(WordBucket*);

#endif
