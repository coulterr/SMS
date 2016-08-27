#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdlib.h>
#include <pthread.h>
#define NULL 0

typedef struct Queue{
        void *nodes;
        int first;
        int size;
        int capacity;
        int unit_size;
        pthread_mutex_t *lock;
}Queue;

void initqueue(Queue *queue, int init_capacity, int unit_size);
int enqueue(Queue *queue, void *node);
int dequeue(Queue *queue, void *dest);
int destroy_queue(Queue *queue);


#endif // QUEUE_H_INCLUDED
