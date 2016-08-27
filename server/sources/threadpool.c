#include "../headers/threadpool.h"

int init_threadpool(Threadpool *threadpool, int thread_capacity, int task_init_capacity)
{
        Queue *taskqueue = malloc(sizeof(Queue));
        initqueue(taskqueue, task_init_capacity, sizeof(Task));
        threadpool->taskqueue = taskqueue;

        sem_t *semaphore = malloc(sizeof(sem_t));
        sem_init(semaphore, 0, 0);
        threadpool->semaphore = semaphore;

        pthread_t *threads = malloc(sizeof(pthread_mutex_t) * thread_capacity);
        int i;
        for(i = 0; i < thread_capacity; i++)
        {
                pthread_create(&threads[i], NULL, (void *)(&work_op), (void *)threadpool);
        }
        threadpool->threads = threads;
}



int work_op(void *args)
{
        Threadpool *threadpool = (Threadpool *) args;
        for(;;)
        {
                sem_wait(threadpool->semaphore);
                {
                        Task *task_ptr = malloc(sizeof(Task));
                        dequeue(threadpool->taskqueue, (void *)task_ptr);
                        //int (*function_ptr)(void *args);
                        //function_ptr = (int(*)(void *))  (task_ptr->function);
                        //function_ptr(task_ptr->args);
                        handle_connection(task_ptr->args);
                }
        }
}


void add_task(Threadpool *threadpool, void *function, void *args)
{
        Task *task = malloc(sizeof(Task));
        //task->function = function;
        task->args = args;
        fprintf(stderr, "%d\n",( (handler_args *)args )->client_FD);
        enqueue(threadpool->taskqueue, (void *)task);

        sem_post(threadpool->semaphore);
}




