#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>
#define SIZEOF_PTR 8
#define DB_NAME "SMS.db"


#include "queue.h"
#include "threadpool.h"
#include "sms_pipe.h"
#include "connection_handler.h"


#endif // MAIN_H_INCLUDED
