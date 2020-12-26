#include "acutest.h"
#include <string.h>

#include "keyvalue.h"
#include "hashtable.h"
#include "clique.h"
#include "specs.h"
#include "parser.h"
#include "negative-cliques.h"
#include "create-output.h"


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


  insertClique(&clique1, specs1);
  insertClique(&clique1, specs2);

  TEST_ASSERT(clique1->list!=NULL);
  TEST_ASSERT(clique1->size==2);

  TEST_ASSERT(clique1->list->next!=NULL);



	deleteClique(clique1);


}

void test_Bucket(void) {

	Bucket* bucket = NULL;

  bucket = createBucket(2);

  TEST_ASSERT(bucket->max==2);
	TEST_ASSERT(bucket->data[0]->flag==0);
	TEST_ASSERT(bucket->data[1]->flag==0);
  TEST_ASSERT(bucket->data[0]->clique==NULL);
	TEST_ASSERT(bucket->data[1]->clique==NULL);
	TEST_ASSERT(bucket->next==NULL);

  Value* value1 = NULL;
  Value* value2 = NULL;
  insertValue(&value1, "val");
  insertValue(&value1, "*");
  KV_Pair* pair1= createKV("key1", value1);


  Specs* specs1 = createSpecs("id1");
  insertSpecs(&specs1, pair1);

  BucketData* input=searchBucket(bucket, specs1->id);
	TEST_ASSERT(!strcmp(input->id, "-"));

	Clique* cl1= createClique();
	insertClique(&cl1, specs1);

	TEST_ASSERT(isInClique(specs1->id,cl1));

	free(input->id);
	input->id=strdup(specs1->id);
	input->clique=cl1;

	input=searchBucket(bucket, specs1->id);

	TEST_ASSERT(!strcmp(input->id, specs1->id));


	deleteBucket(bucket);
	deleteSpecs(specs1);

}

void test_Hashtable(void) {


  Value* value1 = NULL;
  insertValue(&value1, "val");
  Value* value2 = NULL;
  insertValue(&value2, "*");
	Value* value3 = NULL;
  insertValue(&value3, "oki");
  KV_Pair* pair1= createKV("key", value1);
  KV_Pair* pair2= createKV("key", value2);
	KV_Pair* pair3= createKV("key", value3);


  Specs* specs1 = createSpecs("id1");
  insertSpecs(&specs1, pair1);

  Specs* specs2 = createSpecs("id2");
  insertSpecs(&specs2, pair2);

	Specs* specs3 = createSpecs("id3");
  insertSpecs(&specs3, pair3);

  Hashtable* table =NULL;
  table = createHashtable(10,2);

  TEST_ASSERT(table!=NULL);
  TEST_ASSERT(table->max==10);
	for(int i=0; i<table->max;i++){
		TEST_ASSERT(table->array[i]->max==2);
	}

	BucketData* data = searchHashtable(table, specs1->id);
	TEST_ASSERT(data->clique==NULL);
	data = searchHashtable(table, specs2->id);
	TEST_ASSERT(data->clique==NULL);
	data = searchHashtable(table, specs3->id);
	TEST_ASSERT(data->clique==NULL);

  insertHashtable(&table, specs1);
  insertHashtable(&table, specs2);
	insertHashtable(&table, specs3);

	data = searchHashtable(table, specs1->id);
	TEST_ASSERT(data->clique!=NULL);
	TEST_ASSERT(!strcmp(data->id,specs1->id));

	mergeCliques(table, specs1->id, specs2->id);
	mergeCliques(table, specs3->id, specs2->id);


	BucketData* data1 = searchHashtable(table, specs1->id);
	TEST_ASSERT(data->flag==0);
  BucketData* data2 = searchHashtable(table, specs2->id);
	TEST_ASSERT(data2->flag==1);
	BucketData* data3 = searchHashtable(table, specs3->id);
	TEST_ASSERT(data3->flag==1);

  TEST_ASSERT(compareCliques(data1->clique,data2->clique));
	TEST_ASSERT(compareCliques(data1->clique,data3->clique));
	TEST_ASSERT(compareCliques(data2->clique,data3->clique));


	deleteSpecs(specs1);
	deleteSpecs(specs2);
	deleteSpecs(specs3);
	deleteHashtable(table);



}

TEST_LIST = {
	{ "keyvalue", test_KV },
	{ "specs", test_Specs },
	{ "clique", test_Clique },
	{ "bucket", test_Bucket },
	{ "hashtable", test_Hashtable },
	{ NULL, NULL }
};
