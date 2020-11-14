#ifndef specs_h
#define specs_h

typedef struct KV_PairType {
  /*
  A structure to store a key-value pair.
  */
  char* key;
  char* value;
} KV_Pair;

typedef struct SpecsNodeType{
  /*
  This structure is a list of key-value pairs,
  and is furtherly being used in the struct Specs
  */
  KV_Pair* data;
  struct SpecsNodeType *next;
} SpecsNode;

typedef struct SpecsType{
  /*
  This is the Specs structure, where the .json files are
  going to be saved. It consists of the file's id, the list
  of key-value pairs, and an int to keep track of the size, which
  may, or may not, be useful later.
  */
  SpecsNode* list;
  int size;
  char* id;
} Specs;

KV_Pair* createKV(char*, char*);
void deleteKV(KV_Pair*);
char* createSpecsID(char*, char*);
void deleteSpecsId(char*);
Specs* createSpecs(char*);
void insertSpecs(Specs**, char*, char*);
void deleteSpecsList(SpecsNode *);
void deleteSpecs(Specs*);

#endif
