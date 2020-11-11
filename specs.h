#ifndef specs_h
#define specs_h

typedef struct KV_PairType {
  char* key;
  char* value;
} KV_Pair;

typedef struct SpecsNodeType{
  KV_Pair* data;
  struct SpecsNodeType *next;
} SpecsNode;

typedef struct SpecsType{
  SpecsNode* head;
  int size;
  char* id;
} Specs;

KV_Pair* createKV(char*, char*);
void deleteKV(KV_Pair*);
Specs* createSpecsList(char*);
char* createSpecsID(char*, char*);
void insertSpecs(Specs**, char*, char*);

#endif
