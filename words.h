#ifndef word_h
#define word_h

typedef struct WordType{
  /*
  This is the struct of a word inside the vocabulary. It consists of the
  string of the word and some numbers that are explained below.
  */
  char* str;
  int counter; //number of files that the word appears
  double tf_sum; //sum of all the tfs from all different files. used to calculate
  double tfidf_score;//the score that will be used to shrink the table. its the average tf-idf for each word
  int index;
}Word;

typedef struct VocBucketType{
  /*
  A bucket for the hashtable of the vocabulary
  */
  int max;
  Word** words;
  struct VocBucketType* next;
}VocBucket;


Word* createWord(char*);
void deleteWord(Word*);
VocBucket* createVocBucket(int);
void updateTfIdfScore(VocBucket *,int);
Word* searchVocBucket(VocBucket *, char*);
void printVocBucket(VocBucket*);
void deleteVocBucket(VocBucket*);

#endif
