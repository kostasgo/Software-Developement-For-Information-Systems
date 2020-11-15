#ifndef keyvalue_h
#define keyvalue_h

typedef struct ValueType{
  /*
  A struct to store a list of values
  */
  char* str;
  struct ValueType* next;
}Value;

typedef struct KV_PairType {
  /*
  A structure to store a key-value pair.
  */
  char* key;
  Value* value;
} KV_Pair;

void insertValue(Value**, char*);
void deleteValue(Value*);
void printValue(Value*);
KV_Pair* createKV(char*, Value*);
void printKV(KV_Pair*);
void deleteKV(KV_Pair*);

#endif
