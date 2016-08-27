#include "../headers/syncComm.h"


int sendMessage(int socketFD, char *message)
{
        char buffer[1];
        bzero(buffer, 1);
        if(write(socketFD, message, strlen(message)) < 0)
        {
                fprintf(stderr, "COULD NOT WRITE");
                return -1;
        }
        if(read(socketFD, buffer, 1) < 0)
        {
                fprintf(stderr, "COULD NOT RECEIVE1\n");
                return -1;
        }
}



int receiveMessage(int socketFD, char *messageDest)
{
        bzero(messageDest, 256);
        if(read(socketFD, messageDest, 255) < 0)
        {
                fprintf(stderr, "COULD NOT RECEIVE2\n");
                return -1;
        }
        if(write(socketFD, "#", 1) < 0)
        {
                return -1;
        }
}










