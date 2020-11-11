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
  struct SpecsNode* head;
  int size;
  char* id;
} Specs;

#endif
