#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int getAllContacts(int serverFD);
int getUpdatedContacts(int serverFD);
int getAllByContact(int serverFD, char *name);
int getNewByContact(int serverFD, char *name);
int addSending(int serverFD, char *name, char *body);
int sendMessage(int socketFD, char *message);
int receiveMessage(int socketFD, char *messageDest);

int main(int argc, char *argv[])
{
        int serverFD, portNum, len;
        struct sockaddr_in serverAddress;
        struct hostent *server;

	char *hostname = argv[1];
        portNum = atoi(argv[2]);
        serverFD = socket(AF_INET, SOCK_STREAM, 0);
        server = gethostbyname(hostname);



        bzero( (char *) &serverAddress, sizeof(serverAddress) );
        serverAddress.sin_family = AF_INET;
        bcopy( (char *) server -> h_addr, (char *) &serverAddress.sin_addr.s_addr, server -> h_length);
        serverAddress.sin_port = htons(portNum);

        if(connect(serverFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0){
                fprintf(stderr, "ERROR: Could not connect.");
                exit(0);
        }

        if(strcmp(argv[3], "GET_ALL_CONTACTS") == 0){
                getAllContacts(serverFD);

        }else if(strcmp(argv[3], "GET_NEW_CONTACTS") == 0){
                getNewContacts(serverFD); 

        }else if(strcmp(argv[3], "GET_UPDATED_CONTACTS") == 0){
                getUpdatedContacts(serverFD);

        }else if(strcmp(argv[3], "GET_ALL_BY_CONTACT") == 0){
                getAllByContact(serverFD, argv[4]);

        }else if(strcmp(argv[3], "GET_NEW_BY_CONTACT") == 0){
                getNewByContact(serverFD, argv[4]);

        }else if(strcmp(argv[3], "ADD_SENDING") == 0){
                addSending(serverFD, argv[4], argv[5]);

        }else if(strcmp(argv[3], "ADD_SENT") == 0){
		addSent(serverFD, argv[4], argv[5]);

	}else if(strcmp(argv[3], "ADD_RECEIVED") == 0){
		addReceived(serverFD, argv[4], argv[5]);

	}else if(strcmp(argv[3], "ADD_CONTACT") == 0){
		addContact(serverFD, argv[4]); 
	}	
        close(serverFD);

}


int getAllContacts(int serverFD){

        sendMessage(serverFD, "GET_ALL_CONTACTS");
        char buffer[1000];
        for(;;)
        {
                receiveMessage(serverFD, buffer);
                if(strcmp(buffer, "#close") == 0){
                        break;
                }else{
                        printf("%s\n", buffer);
                }
        }
}

int getNewContacts(int serverFD){ 
        
        sendMessage(serverFD, "GET_NEW_CONTACTS"); 
        char buffer[1000];
        for(;;)
        {
                receiveMessage(serverFD, buffer); 
                if(strcmp(buffer, "#close") == 0){
                        break;
                }else{
                        printf("%s\n", buffer); 
                }
        }
}


int getUpdatedContacts(int serverFD){

        sendMessage(serverFD, "GET_UPDATED_CONTACTS");
        char buffer[1000];
        for(;;)
        {
                receiveMessage(serverFD, buffer);
                if(strcmp(buffer, "#close") == 0){
                        break;
                }else{
                        printf("%s\n", buffer);
                }
        }
}

int getAllByContact(int serverFD, char *name){

        sendMessage(serverFD, "GET_ALL_BY_CONTACT");
        sendMessage(serverFD, name);
        char status[180]; 
        for(;;)
        {
                receiveMessage(serverFD, status);
                if(strcmp(status, "#close") == 0){
                        break;
                }else{
			char timestamp[180];
			receiveMessage(serverFD, timestamp);
			char sender[180];
			receiveMessage(serverFD, sender);
			char body[180];
			receiveMessage(serverFD, body);  
			char output[1000]; 
			bzero(output, 1000); 
			if(strcmp(status, "received") == 0){
				strcat(output, sender); 
				strcat(output, ": "); 
				strcat(output, body); 
                        	printf("%s\n", output);
			}else if(strcmp(status, "sent") == 0){
				strcat(output, "Me: "); 
				strcat(output, body); 
				printf("%s\n", output); 
			}
                }
        }
}

int getNewByContact(int serverFD, char *name){

        sendMessage(serverFD, "GET_NEW_BY_CONTACT");
        sendMessage(serverFD, name);
        char status[180]; 
        for(;;)
        {
                receiveMessage(serverFD, status);
                if(strcmp(status, "#close") == 0){
                        break;
                }else{
			char timestamp[180];
			receiveMessage(serverFD, timestamp);
			char sender[180];
			receiveMessage(serverFD, sender);
			char body[180];
			receiveMessage(serverFD, body);  
			char output[1000]; 
			bzero(output, 1000); 
			if(strcmp(status, "received") == 0){
				strcat(output, sender); 
				strcat(output, ": "); 
				strcat(output, body); 
                        	printf("%s\n", output);
			}else if(strcmp(status, "sent") == 0){
				strcat(output, "Me: "); 
				strcat(output, body); 
				printf("%s\n", output); 
			}
                }
        }
}

int addSending(int serverFD, char *name, char *body){

        sendMessage(serverFD, "ADD_SENDING");
        sendMessage(serverFD, name);
        sendMessage(serverFD, body);
        char buffer[256];
        receiveMessage(serverFD, buffer);
}

int addSent(int serverFD, char *name, char *body){
	sendMessage(serverFD, "ADD_SENT");
	sendMessage(serverFD, name);
	sendMessage(serverFD, body);
	char buffer[256];
	receiveMessage(serverFD, buffer);
}

int addReceived(int serverFD, char *name, char *body){
	sendMessage(serverFD, "ADD_RECEIVED");
	sendMessage(serverFD, name);
	sendMessage(serverFD, body);
	char buffer[256];
	receiveMessage(serverFD, buffer);
}

int addContact(int serverFD, char *name)
{
	sendMessage(serverFD, "ADD_CONTACT"); 
	sendMessage(serverFD, name); 
}


int sendMessage(int socketFD, char *message)
{
        char buffer[1];
        bzero(buffer, 1);
        if(write(socketFD, message, strlen(message)) < 0){
                return -1;
        }
        if(read(socketFD, buffer, 1) < 0){
                return -1;
        }
}

int receiveMessage(int socketFD, char *messageDest)
{
        bzero(messageDest, 256);
        if(read(socketFD, messageDest, 255) < 0){
                return -1;
        }
        if(write(socketFD, "#", 1) < 0){
                return -1;
        }
}

