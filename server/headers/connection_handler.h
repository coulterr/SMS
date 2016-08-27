#ifndef CONNECTION_HANDLER_H_INCLUDED
#define CONNECTION_HANDLER_H_INCLUDED
#include "sms_pipe.h"
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

typedef struct handler_args
{
        sms_pipe *incoming_pipe;
        sms_pipe *outgoing_pipe;
        pthread_mutex_t *db_lock;
        char *outgoing_timestamp;
        int client_FD;
}handler_args;


int handle_connection(void *arguments);

#endif // CONNECTION_HANDLER_H_INCLUDED
