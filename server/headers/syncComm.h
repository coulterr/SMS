#ifndef SYNCCOMM_H_INCLUDED
#define SYNCCOMM_H_INCLUDED

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <sys/time.h>

int sendMessage(int socketFD, char *message);
int receiveMessage(int socketFD, char *messageDest);
int getResponse(int socketFD);

#endif
