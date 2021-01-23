#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct threadpool_ * threadpool;

int threadpool_create(threadpool*, int, int);
int threadpool_add(threadpool, void (*function)(void *),void *arg);
int threadpool_exit(threadpool);
int threadpool_free(threadpool);
void *job_scheduler(void*);

#endif