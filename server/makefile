SERVERMAKE: sources/main.c sources/connection_handler.c sources/queue.c sources/sms_pipe.c sources/syncComm.c sources/threadpool.c sources/timestamp_creator.c
	gcc -c sources/main.c -o objects/main.o
	gcc -c sources/connection_handler.c -o objects/connection_handler.o
	gcc -c sources/queue.c -o objects/queue.o
	gcc -c sources/sms_pipe.c -o objects/sms_pipe.o
	gcc -c sources/syncComm.c -o objects/syncComm.o
	gcc -c sources/threadpool.c -o objects/threadpool.o
	gcc -c sources/timestamp_creator.c -o objects/timestamp_creator.o
	gcc objects/main.o objects/connection_handler.o objects/queue.o objects/sms_pipe.o objects/syncComm.o \
	objects/threadpool.o objects/timestamp_creator.o -lpthread -lsqlite3 -o server
