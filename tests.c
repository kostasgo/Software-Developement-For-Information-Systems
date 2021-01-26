#include "acutest.h"
#include <string.h>
#include "bucket.h"
#include "keyvalue.h"
#include "hashtable.h"
#include "clique.h"
#include "specs.h"
#include "parser.h"
#include "lowlevel-io.h"
#include "negative-cliques.h"
#include "create-output.h"
#include "bow.h"
#include "thread_pool.h"

// global vars for threads
int task = 0;
pthread_mutex_t lock;

void test_t(void *arg) {
	pthread_mutex_lock(&lock);
	task++;
	pthread_mutex_unlock(&lock);
}

void one_thread_one_job(void) {
	threadpool tp;

	int threads = 1;
	int queue_size = 10;

	TEST_ASSERT(threadpool_create(&tp, threads, queue_size) == 0);


	// Add a test task
	TEST_ASSERT(threadpool_add(tp, &test_t, NULL) == 0);




	TEST_ASSERT(threadpool_exit(tp) == 0);

}


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


void test_LLIO(void) {

	int numOfDirs = countDirectories("Datasets/2013_camera_specs");
	TEST_ASSERT(numOfDirs == 24);   // Test if the subdirectories are really 24

	char* path = createPath("Datasets/2013_camera_specs", "buy.net");
	TEST_ASSERT(!strcmp(path, "Datasets/2013_camera_specs/buy.net")); // Test if the sring of the path was created as expected

	deletePath(path);


	char** directories = NULL;
	directories = createDirTable(numOfDirs, "Datasets/2013_camera_specs");
	TEST_ASSERT(directories != NULL);  // Test if the array of directories is not NULL

	deleteDirTable(directories, numOfDirs);

}


void test_Parser(void){

	Specs* test = parser("buy.net", "4233.json");

	SpecsNode* node;
	node = test->list;
	KV_Pair * pair;
	pair = node->data;


	// Test if the first pair of values of the spec is as expected
	TEST_ASSERT(!strcmp(pair->key, "<page title>") && !strcmp(pair->value->str, "Olympus OM-D E-M10 Black Digital Camera (16.1 MP, SD/SDHC/SDXC Card Slot) Price Comparison at Buy.net"));


	while(node->next != NULL)
		node = node->next;

	pair = node->data;

	// Test if the last pair of values of the spec is as expected
	TEST_ASSERT(!strcmp(pair->key, "wifi") && !strcmp(pair->value->str, "Yes"));


	deleteSpecs(test);

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


void test_Vocabulary(void){

	char* str = strdup("HeLl0 WoRld!");
	toLower(str);
	TEST_ASSERT(!strcmp(str, "hell0 world!"));
	free(str);


	int i;
	char** stopWords = createStopWordsTable();

	TEST_ASSERT(isStopword("you", stopWords) && isStopword("also", stopWords) && isStopword("among", stopWords) || !isStopword("samsung", stopWords));


	Word* word = createWord(str);

	if(word->counter == 0 && word->index == -1 && word->tf_sum == 0 && word->tfidf_score == 0 && word->idf == 0 && !strcmp(str, word->str))

	deleteWord(word);

	i = 6;


    VocBucket* vocBuc = createVocBucket(i);
	TEST_ASSERT(vocBuc->max == i && vocBuc->next == NULL && !strcmp(vocBuc->words[0]->str, "-") && !strcmp(vocBuc->words[i-1]->str, "-"));

	vocBuc->words[0]->str = strdup("firstword");

	word = createWord(searchVocBucket(vocBuc, "-")->str);
	TEST_ASSERT(!strcmp(word->str, "-"));
	deleteWord(word);

	word = createWord(searchVocBucket(vocBuc, "firstword")->str);
	TEST_ASSERT(!strcmp(word->str, "firstword"));
	deleteWord(word);

	word = createWord(searchVocBucket(vocBuc, "secondword")->str);
	TEST_ASSERT(!strcmp(word->str, "-"));
	deleteWord(word);

	deleteVocBucket(vocBuc);



	Vocabulary* voc = NULL;
	voc = createVocabulary(6, 4);
	TEST_ASSERT(voc != NULL);


	insertVocabulary(&voc, "testWord", 1.0);
	insertVocabulary(&voc, "testWord2", 1.0);
	TEST_ASSERT(!strcmp((searchVocabulary(voc, "testWord")->str), "testWord"));
	TEST_ASSERT(!strcmp((searchVocabulary(voc, "testWord2")->str), "testWord2"));

	deleteVocabulary(voc);

	voc = createVocabulary(100, 5);

	Specs* spec = parser("buy.net", "4233.json");
	SpecsNode* specNode = spec->list;

	filterSpec(spec, stopWords);

	ListNode* specsList = NULL;

	insertList(&specsList, spec);

	spec = parser("www.alibaba.com", "5445.json");
	filterSpec(spec, stopWords);
	insertList(&specsList, spec);

	spec = parser("buy.net", "4233.json");
	filterSpec(spec, stopWords);
	insertList(&specsList, spec);

	int totalSpecs = fillVocabulary(&voc, specsList);
	updateScores(voc, totalSpecs);

	deleteStopWordsTable(stopWords);

	int bowSize=-1;
    Word** bow=  shrinkTable(voc, 0.0029, &bowSize);

    for(i = 0; i < bowSize; i++){
        TEST_ASSERT(!strcmp((searchVocabulary(voc, bow[i]->str)->str), bow[i]->str));
    }

	deleteVocabulary(voc);

}

TEST_LIST = {
	{ "one_thread_one_job", one_thread_one_job},
	{ "keyvalue", test_KV },
	{ "specs", test_Specs },
	{ "clique", test_Clique },
	{ "bucket", test_Bucket },
	{ "hashtable", test_Hashtable },
	{ "lowlevel-io", test_LLIO },
	{ "parser", test_Parser },
	{ "vocabulary", test_Vocabulary },
	{ NULL, NULL }
};
