#ifndef SMS_PIPE_H_INCLUDED
#define SMS_PIPE_H_INCLUDED
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "queue.h"
#define SIZEOF_PTR 8
typedef struct sms_pipe
{
    Queue *message_queue;
    sem_t *sem;
    pthread_mutex_t *db_lock;
    char *db_name;
}sms_pipe;

typedef struct sms_message
{
        char *status;
        char *name;
        char *body;
}sms_message;

int sms_pipe_init (sms_pipe *pipe, pthread_mutex_t *db_lock, char *db_name);
int wait_for_sms (void *arguments);
int add_to_pipe(sms_pipe *pipe, sms_message *message);



#endif // SMS_PIPE_H_INCLUDED
