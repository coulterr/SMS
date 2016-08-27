#include "../headers/queue.h"


void initqueue(Queue *queue, int init_capacity, int unit_size)
{
        queue->nodes = malloc(unit_size * init_capacity);
        queue->first = 0;
        queue->size = 0;
        queue->capacity = init_capacity;
        queue->unit_size = unit_size;

        pthread_mutex_t *mutex = malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(mutex, NULL);
        pthread_mutex_unlock(mutex);
        queue->lock = mutex;
}



int enqueue(Queue *queue, void *node)
{
        pthread_mutex_lock(queue->lock);
        {
                if(queue->size == queue->capacity)
                {
                        char *head = (char *) queue->nodes + (queue->first * queue->unit_size);
                        queue->capacity *= 2;
                        void *temp = malloc(queue->capacity * queue->unit_size);
                        memcpy(temp, (void *) head, queue->unit_size * queue->size);
                        free(queue->nodes);
                        queue->nodes = temp;
                        queue->first = 0;
                }

                char *head = (char *) queue->nodes + (queue->first * queue->unit_size);
                char *tail = head + (queue->size * queue->unit_size);
                memcpy((void *) tail, node, queue->unit_size);
                queue->size++;
        }
        pthread_mutex_unlock(queue->lock);
}



int dequeue(Queue *queue, void *dest)
{
        pthread_mutex_lock(queue->lock);
        {

                if(queue->size == 0)
                {
                        pthread_mutex_unlock(queue->lock);
                        return -1;
                }
                char *head = (char *) (queue->nodes) + (queue->first * queue->unit_size);
                memcpy(dest, (void *) head, queue->unit_size);
                queue->size--;
                queue->first++;
                if(queue->size * 4 < queue->capacity)
                {
                        queue->capacity /= 2;
                        void *temp = malloc(queue->capacity * queue->unit_size);
                        head = (char *) queue->nodes + (queue->first * queue->unit_size);
                        memcpy(temp, (void *) head, queue->size * queue->unit_size);
                        free(queue->nodes);
                        queue->nodes = temp;
                        queue->first = 0;
                }
        }
        pthread_mutex_unlock(queue->lock);
}



int destroy_queue(Queue *queue)
{
        free(queue->nodes);
        free(queue);
}



