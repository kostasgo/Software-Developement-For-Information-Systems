#include "acutest.h"
#include <string.h>

#include "keyvalue.h"
#include "hashtable.h"
#include "clique.h"
#include "specs.h"
#include "parser.h"


void test_KV(void) {

	Value* value = NULL;

  insertValue(&value, "val1");
  insertValue(&value, "val2");

	TEST_ASSERT(value != NULL); //test its not NULL
  TEST_ASSERT(!strcmp(value->next->str,"val2")); // test if the second entry is right

  KV_Pair* pair = NULL;
  pair = createKV("key",value);

	TEST_ASSERT(pair != NULL);

	deleteKV(pair);




}

void test_Specs(void) {

	Specs* specs = NULL;

  specs = createSpecs("id");

  TEST_ASSERT(specs->size==0);
  TEST_ASSERT(specs->list==NULL);

  Value* value =NULL;
  insertValue(&value, "val");
  KV_Pair* pair= createKV("key", value);


  insertSpecs(&specs, pair);



  TEST_ASSERT(specs->size==1);
  TEST_ASSERT(specs->list!=NULL);
  TEST_ASSERT(!strcmp(specs->list->data->key,"key"));


	deleteSpecs(specs);


}

void test_Clique(void) {

	Clique* clique1 = NULL;

  clique1 = createClique();

  TEST_ASSERT(clique1->size==0);
  TEST_ASSERT(clique1->list==NULL);

  Value* value1 = NULL;
  Value* value2 = NULL;
  insertValue(&value1, "val");
  insertValue(&value1, "*");
  KV_Pair* pair1= createKV("key1", value1);
  KV_Pair* pair2= createKV("key2", value2);

  Specs* specs1 = createSpecs("id1");
  insertSpecs(&specs1, pair1);

  Specs* specs2 = createSpecs("id2");
  insertSpecs(&specs2, pair2);

  Clique* clique2 = createClique();

  insertClique(&clique1, specs1);
  insertClique(&clique2, specs2);

  TEST_ASSERT(clique1->list!=NULL);
  TEST_ASSERT(clique1->size==1);

  mergeCliques(clique1, clique2);

  TEST_ASSERT(clique1->size==2);
  TEST_ASSERT(clique1->list->next!=NULL);
  TEST_ASSERT(clique2->list==NULL);


	deleteClique(clique1);


  deleteClique(clique2);

}

void test_Bucket(void) {

	Bucket* bucket = NULL;

  bucket = createBucket(2);

  TEST_ASSERT(bucket->max==2);
  TEST_ASSERT(bucket->cliques[0]->list==NULL);
	TEST_ASSERT(bucket->cliques[1]->list==NULL);
	TEST_ASSERT(bucket->next==NULL);

  Value* value1 = NULL;
  Value* value2 = NULL;
  insertValue(&value1, "val");
  insertValue(&value1, "*");
  KV_Pair* pair1= createKV("key1", value1);
  KV_Pair* pair2= createKV("key2", value2);

  Specs* specs1 = createSpecs("id1");
  insertSpecs(&specs1, pair1);

  Specs* specs2 = createSpecs("id2");
  insertSpecs(&specs2, pair2);

  insertBucket(&bucket, specs1);
  insertBucket(&bucket, specs2);

  TEST_ASSERT(bucket->cliques[0]!=NULL);
  TEST_ASSERT(bucket->cliques[1]!=NULL);

	TEST_ASSERT(!strcmp(bucket->cliques[0]->list->specs->id,"id1"));


	deleteBucket(bucket);

}

void test_Hashtable(void) {


  Value* value1 = NULL;
  insertValue(&value1, "val");
  Value* value2 = NULL;
  insertValue(&value2, "*");
  KV_Pair* pair1= createKV("key", value1);
  KV_Pair* pair2= createKV("key", value2);


  Specs* specs1 = createSpecs("id1");
  insertSpecs(&specs1, pair1);

  Specs* specs2 = createSpecs("id2");
  insertSpecs(&specs2, pair2);

  Hashtable* table =NULL;
  table = createHashtable(2,2);

  TEST_ASSERT(table!=NULL);
  TEST_ASSERT(table->max==2);
	TEST_ASSERT(table->array[0]->max==2);
	TEST_ASSERT(table->array[1]->max==2);

  insertHashtable(&table, specs1);
  insertHashtable(&table, specs2);

  Clique* cl1 = searchHashtable(table, "id1");
  Clique* cl2 = searchHashtable(table, "id2");

  TEST_ASSERT(!compareCliques(cl1,cl2));

  deleteHashtable(table);



}

TEST_LIST = {
	{ "keyvalue", test_KV },
	{ "specs", test_Specs },
	{ "clique", test_Clique },
	{ "bucket", test_Bucket },
	{ NULL, NULL }
};
