#ifndef vocabulary_h
#define vocabulary_h

typedef struct VocTable{
  
}

void toLower(char*);
char** createStopWordsTable();
void deleteStopWordsTable(char**);
int isStopword(char*, char**);
void filterSpec(Specs*, char**);

#endif
