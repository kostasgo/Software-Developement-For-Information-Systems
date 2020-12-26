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


Word* createWord(char*);
void deleteWord(Word*);
VocBucket* createVocBucket(int);
VocBucket* createVocBucket(int);
void printVocBucket(VocBucket*);
void deleteVocBucket(VocBucket*);

#endif
