#include "thread_pool.h"

typedef struct {
    void (*function)(void *);
    void *argument;
} task;

struct threadpool_ {
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *threads;
  task*queue;
  int thread_count;
  int queue_size;
  int head;                 // Current task queue head
  int tail;                 // End of task queue
  int count;                // Number of tasks left in queue
  int shutdown;             // Shutdown flag
  int started;              // Number of threads running
};

void *job_scheduler(void *t_p) {
    threadpool tp;
    tp = (threadpool)t_p;
    task task;

    while(1) {
        pthread_mutex_lock(&(tp->lock));

        while((tp->count == 0) && (!tp->shutdown)) {
            // task queue is empty and thread pool is blocked when it is not closed
            pthread_cond_wait(&(tp->notify), &(tp->lock));
        }
        if(tp->shutdown) break;

        // get task from queue
        task.function = tp->queue[tp->head].function;
        task.argument = tp->queue[tp->head].argument;

        tp->head += 1;
        tp->head = (tp->head == tp->queue_size) ? 0 : tp->head;
        tp->count -= 1;

        // Unlock
        pthread_mutex_unlock(&(tp->lock));

        // Execute the function
        (*(task.function))(task.argument);
    }

    // One less job
    tp->started--;

    //Unlock
    pthread_mutex_unlock(&(tp->lock));
    pthread_exit(NULL);
}

int threadpool_create(threadpool *tp, int thread_count, int queue_size) {
    if (thread_count < 1 || queue_size < 1) {
        fprintf(stderr, "Thread number and queue size must be positive and non-zero\n");
        return -1;
    }

    if((*tp = (threadpool)malloc(sizeof(struct threadpool_))) == NULL) {
        fprintf(stderr, "Error  allocating thread_pool memory\n");
        return -1;
    }

    // Initialize tread pool state variables
    (*tp)->queue_size = queue_size;
    (*tp)->head = (*tp)->tail = (*tp)->count = 0;
    (*tp)->shutdown = (*tp)->started = 0;

    // Allocate thread and task queue
    (*tp)->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
    (*tp)->queue = (task *)malloc(sizeof(task) * queue_size);

    if (pthread_mutex_init(&((*tp)->lock), NULL) != 0) {
        fprintf(stderr, "Posix mutex init error\n");
        free(*tp);
        return -1;
    }
    if (pthread_cond_init(&((*tp)->notify), NULL) != 0 ) {
        fprintf(stderr, "Posix conditional variable init error\n");
        free(*tp);
        return -1;
    }

    // Start specified threads
    for(int i = 0; i < thread_count; i++) {
        if(pthread_create(&((*tp)->threads[i]), NULL, job_scheduler, (void*)(*tp)) != 0) {
            fprintf(stderr, "Failed to create thread %d\n", i);
            threadpool_exit(*tp);
            return -1;
        }
        (*tp)->thread_count++;
        (*tp)->started++;
    }

    return 0;
}

int threadpool_add(threadpool tp, void (*function)(void *),void *arg) {
    if(tp == NULL || function == NULL) {
        fprintf(stderr, "Empty thread pool or function\n");
        return -1;
    }

    // lock
    if(pthread_mutex_lock(&(tp->lock)) != 0) {
        fprintf(stderr, "Mutex lock error.\n");
        return -3;
    }

    int next = tp->tail + 1;
    if (next == tp->queue_size) next = 0;

    do {
        // Check if the task queue is full
        if(tp->count == tp->queue_size) {
            fprintf(stderr, "Full queue\n");
            break;
        }
        // If thread pool is on shutdown
        if(tp->shutdown) break;

        // Add task to end of queue
        tp->queue[tp->tail].function = function;
        tp->queue[tp->tail].argument = arg;
        // Update tail and count
        tp->tail = next;
        tp->count += 1;

        // broadcast
        if(pthread_cond_signal(&(tp->notify)) != 0) {
            if(pthread_mutex_unlock(&tp->lock) != 0) {
                fprintf(stderr, "Mutex lock error.\n");
                return -3;
            }
            fprintf(stderr, "Error on broadcast\n");
            return -2;
        }
    } while(0);

    // Release mutex resources
    if(pthread_mutex_unlock(&tp->lock) != 0) {
        fprintf(stderr, "Mutex lock error.\n");
        return -3;
    }

    return 0;
}

int threadpool_exit(threadpool tp) {
    if(tp == NULL) {
        fprintf(stderr, "Empty thread pool\n");
        return -1;
    }

    // Lock
    if(pthread_mutex_lock(&(tp->lock)) != 0) {
        fprintf(stderr, "Mutex lock error\n");
        return -3;
    }

    do {
        if(tp->shutdown) {
            fprintf(stderr, "Thread pool on shutdown\n");
            return -1;
        }

        tp->shutdown = 1;

        // Unlock all
        if((pthread_cond_broadcast(&(tp->notify)) != 0) || (pthread_mutex_unlock(&(tp->lock)) != 0)) {
            fprintf(stderr, "Mutex unlock error");
            return -3;
        }


        // Waiting for all threads to end
        for(int i = 0; i < tp->thread_count; i++) {
            if(pthread_join(tp->threads[i], NULL) != 0) {
                fprintf(stderr, "Join error\n");
            }
        }
    } while(0);

    // Dealocate
    threadpool_free(tp);

    return 0;
}

int threadpool_free(threadpool tp)
{
    if(tp == NULL) {
        fprintf(stderr, "Empty thread pool\n");
        return -1;
    }

    if (tp->started > 0) {
        fprintf(stderr, "Jobs still running\n");
        return -1;
    }

    if(tp->threads) {
        free(tp->threads);
        free(tp->queue);
        pthread_mutex_lock(&(tp->lock));
        pthread_mutex_destroy(&(tp->lock));
        pthread_cond_destroy(&(tp->notify));
    }

    free(tp);
    return 0;
}