#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include "connection_handler.h"
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue.h""


typedef struct Task
{
        void *args;
}Task;

typedef struct Threadpool
{
        Queue *taskqueue;
        pthread_t *threads;
        sem_t *semaphore;
}Threadpool;


int init_threadpool(Threadpool *threadpool, int thread_capacity, int task_init_capacity);
int work_op(void *args);
void add_task(Threadpool *threadpool, void *function, void *args);

#endif // THREADPOOL_H_INCLUDED
