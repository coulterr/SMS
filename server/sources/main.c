#include "../headers/main.h"

int main(int argc, char **argv)
{
        int server_fd;
        struct sockaddr_in client_addr, server_addr;
        socklen_t client_len;

        if(argc < 2)
        {
                fprintf(stderr, "ERROR: PORTS NOT SPECIFIED\n");
                exit(1);
        }

        int port_num = atoi(argv[1]);

        server_fd = socket(AF_INET, SOCK_STREAM, 0);

        if(server_fd < 0)
        {
                fprintf(stderr, "ERROR: COULD NOT CREATE SERVER SOCKET\n");
                exit(1);
        }

        //specify address family
        server_addr.sin_family = AF_INET;
        //specify port number
        server_addr.sin_port = htons(port_num);
        //Specify address (accept connections of any of machine IP addresses)
        server_addr.sin_addr.s_addr = INADDR_ANY;

        //bind socket file descriptor to specified local address (IP + port)
        if( bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1)
        {
                fprintf(stderr, "ERROR: COULD NOT BIND SERVER SOCKET TO LOCAL IP\n");
                exit(1);
        }


        //Listen on server socket, up to 300 connection queue
        if( listen(server_fd, 300) == -1)
        {
                fprintf(stderr, "ERROR: COULD NOT LISTEN ON SERVER SOCKET\n");
                exit(1);
        }



        client_len = sizeof(client_addr);
        fprintf(stderr, "Listening for clients...\n");

        pthread_mutex_t db_lock;
        pthread_mutex_init(&db_lock, NULL);
        pthread_mutex_unlock(&db_lock);

        sms_pipe incoming_pipe;;
        sms_pipe outgoing_pipe;
        sms_pipe_init(&incoming_pipe, &db_lock, DB_NAME);
        sms_pipe_init(&outgoing_pipe, &db_lock, DB_NAME);


        char incoming_timestamp[23];
        get_timestamp(incoming_timestamp);
        char outgoing_timestamp[23];
        get_timestamp(outgoing_timestamp);
        Threadpool threadpool;
        init_threadpool(&threadpool, 30, 10);

    	struct timeval timeout;      
    	timeout.tv_sec = 10;
    	timeout.tv_usec = 0;

        for(;;)
        {
                handler_args arguments;
                arguments.incoming_pipe = &incoming_pipe;
                arguments.outgoing_pipe = &outgoing_pipe;
                arguments.db_lock = &db_lock;
                arguments.outgoing_timestamp = outgoing_timestamp;
                arguments.client_FD = accept(server_fd, (struct sockaddr *) &client_addr, &client_len);

   		setsockopt (arguments.client_FD, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout));
		setsockopt (arguments.client_FD, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,sizeof(timeout));

                add_task(&threadpool, (void *)&handle_connection, (void *)&arguments);
        }

        return 0;
}
