#ifndef vocabulary_h
#define vocabulary_h

typedef struct VocBucketType{

}

void toLower(char*);
char** createStopWordsTable();
void deleteStopWordsTable(char**);
int isStopword(char*, char**);
void filterSpec(Specs*, char**);

#endif
