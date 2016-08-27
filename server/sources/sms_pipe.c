#include "../headers/sms_pipe.h"




int run_message_insert_query(char *sql)
{
        sqlite3 *db;
        char *zErrMsg = 0;
        int rc;
        const char* data = "Callback function called";

        /* Open database */
        rc = sqlite3_open("SMS.db", &db);
        if( rc )
        {
                fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
                exit(0);
        }
        else
        {
                fprintf(stderr, "Opened database successfully\n");
        }

        /* Execute SQL statement */
        rc = sqlite3_exec(db, sql, 0, (void*)data, &zErrMsg);

        if( rc != SQLITE_OK )
        {
                fprintf(stderr, "SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
        }
        else
        {
                fprintf(stdout, "Operation done successfully\n");
        }

        sqlite3_close(db);
        return 0;
}

int sms_pipe_init (sms_pipe *pipe, pthread_mutex_t *db_lock, char *db_name)
{
        Queue *queue = malloc(sizeof(Queue));
        initqueue(queue, 10, sizeof(sms_message));
        pipe->message_queue = queue;

        sem_t *sem = malloc(sizeof(sem_t));
        sem_init(sem, 0, 0);
        pipe->sem = sem;

        pipe->db_name = db_name;
        pipe->db_lock = db_lock;

        pthread_t listener = malloc(sizeof(pthread_t));
        pthread_create(listener, NULL, (void *)&wait_for_sms, (void *)pipe);

}



int wait_for_sms (void *arguments)
{
        for(;;)
        {
                sms_pipe *pipe = (sms_pipe *) arguments;
                sem_wait(pipe->sem);
                {
                        sms_message *msg = malloc(sizeof(sms_message));
                        dequeue(pipe->message_queue, (void *) msg);

                        //inspection
                        char name[256];
                        bzero(name, 256);
                        fprintf(stderr, "%x", msg->name);
                        memcpy(name, msg->name, 100);
                        int i = 0;
                        for(i=0; i<100; i++){
                        fprintf(stderr, "%c", name[i]);

                        }


                        pthread_mutex_lock(pipe->db_lock);
                        {
                                char timestamp[23];
                                get_timestamp(timestamp);
                                char query[1000];
                                bzero(query, 1000);
                                strcat(query, "INSERT INTO messages VALUES(\"");
                                strcat(query, msg->status);
                                fprintf(stderr, "%s", msg->status);
                                strcat(query, "\", \"");
                                strcat(query, timestamp);
                                fprintf(stderr, "%s", timestamp);
                                strcat(query, "\", \"");
                                fprintf(stderr, "%s", msg->name);
                                strcat(query, msg->name);

                                strcat(query, "\", \"");
                                strcat(query, msg->body);
                                strcat(query, "\");");

                                fprintf(stderr, "%s", query);

                                run_message_insert_query(query);

				if(strcmp(msg->status, "sending") != 0)
				{
					bzero(query, 1000);
					strcat(query, "UPDATE contacts SET lastModDate=\"");
					strcat(query, timestamp); 
					strcat(query, "\" WHERE name=\"");
					strcat(query, msg->name); 
					strcat(query, "\";");
					run_message_insert_query(query); 
				}

				
                        }
                        pthread_mutex_unlock(pipe->db_lock);
                }
        }

}



int add_to_pipe(sms_pipe *pipe, sms_message *message)
{
        fprintf(stderr, "ENQUEUEING\n");
        fprintf(stderr, "%s\n", message->name);
        enqueue(pipe->message_queue, message);
        sem_post(pipe->sem);

}

