/**
Needs to handle following commands:
    1. Add a (incoming or outgoing) SMS to DB
    2. Get sweep of ALL SMS
    3. Get all incoming/outgoing SMS SINCE given TIMESTAMP
        -Should store TIMESTAMP locally?

    COMMANDS:
        -EXPORT (gets all stored messages)
        -UPDATE INCOMING (gets stored incoming/outgoing since last timestamp)
        -UPDATE OUTGOING
        -ADD (generates timestamp and adds to database)
*/

#include "../headers/connection_handler.h"

handler_args *args;

int callback(void *data, int argc, char **argv, char **azColName)
{
        int i;
        fprintf(stderr, "%s: ", (const char*) data);

        for(i=0; i<argc; i++)
        {
                printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
                sendMessage(args->client_FD, argv[i]);
        }
        printf("\n");

        return 0;
}



int run_query(char *sql)
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
        rc = sqlite3_exec(db, sql, &callback, (void*)data, &zErrMsg);

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



int handle_connection(void *arguments)
{
        args = (handler_args *) arguments;

        fprintf(stderr, "%s\n", "Handling connection...");
        fprintf(stderr, "%d\n", args->client_FD);

        char buffer[256];
        receiveMessage(args->client_FD, buffer);
        fprintf(stderr, "%s\n", buffer); 
        if(strcmp(buffer, "GET_ALL_CONTACTS") == 0)
        {
                pthread_mutex_lock(args->db_lock);
                {
                        run_query("SELECT name FROM contacts;");

                }
                pthread_mutex_unlock(args->db_lock);

                sendMessage(args->client_FD, "#close");
                return;
        }
        else if(strcmp(buffer, "GET_NEW_CONTACTS") == 0)
        {
                pthread_mutex_lock(args->db_lock);
                {
                        run_query("SELECT name FROM contacts WHERE noticed = \"false\""); 
                        run_query("UPDATE contacts SET noticed = \"true\" WHERE noticed = \"false\""); 
                }
                pthread_mutex_unlock(args->db_lock); 
                
                sendMessage(args->client_FD, "#close"); 
                return;       
        }
        else if(strcmp(buffer, "GET_UPDATED_CONTACTS") == 0)
        {
                pthread_mutex_lock(args->db_lock);
                {
                        run_query("SELECT name FROM contacts WHERE lastGetDate < lastModDate");

                }
                pthread_mutex_unlock(args->db_lock);

                sendMessage(args->client_FD, "#close");
                return;


        }
        else if(strcmp(buffer, "GET_ALL_BY_CONTACT") == 0)
        {
                char name[256];
                receiveMessage(args->client_FD, name);

                pthread_mutex_lock(args->db_lock);
                {
                        char query[1000];
                        bzero(query, 1000);

                        strcat(query, "SELECT * FROM messages WHERE status != \"sending\" AND name = \"");
                        strcat(query, name);
                        strcat(query, "\" ORDER BY timestamp ASC;");
                        fprintf(stderr, "%s", query);
                        run_query(query);
                        char new_timestamp[23];
                        get_timestamp(new_timestamp);
                        bzero(query, 1000);
                        strcat(query, "UPDATE contacts SET lastGetDate=\"");
                        strcat(query, new_timestamp);
                        strcat(query, "\" WHERE name=\"");
                        strcat(query, name);
                        strcat(query, "\";");
                        run_query(query);

                }
                pthread_mutex_unlock(args->db_lock);

                sendMessage(args->client_FD, "#close");
                return;
        }
        else if(strcmp(buffer, "GET_NEW_BY_CONTACT") == 0){
                char name[256];
                receiveMessage(args->client_FD, name);

                pthread_mutex_lock(args->db_lock);
                {
                        char query[1000];
                        bzero(query, 1000);

                        strcat(query, "SELECT * FROM messages WHERE status != \"sending\" AND name =\"");
                        strcat(query, name);
                        strcat(query, "\" AND timestamp > (SELECT lastGetDate FROM contacts WHERE name=\"");
                        strcat(query, name);
                        strcat(query, "\");");
                        fprintf(stderr, query);
                        run_query(query);

                        char new_timestamp[23];
                        get_timestamp(new_timestamp);
                        bzero(query, 1000);
                        strcat(query, "UPDATE contacts SET lastGetDate=\"");
                        strcat(query, new_timestamp);
                        strcat(query, "\" WHERE name=\"");
                        strcat(query, name);
                        strcat(query, "\";");
                        run_query(query);

                }
                pthread_mutex_unlock(args->db_lock);
                sendMessage(args->client_FD, "#close"); 
                return;

        }
        else if(strcmp(buffer, "GET_SENDING") == 0)
        {
                pthread_mutex_lock(args->db_lock);
                {

                        run_query("SELECT * FROM messages WHERE status=\"sending\";");
                        run_query("DELETE FROM messages WHERE status=\"sending\";");

                }
                pthread_mutex_unlock(args->db_lock);

                sendMessage(args->client_FD, "#close");
                return;
        }

        else if(strcmp(buffer, "ADD_RECEIVED") == 0)
        {

                sms_message *message_ptr = malloc(sizeof(sms_message));

                message_ptr->status = malloc(256 * sizeof(char));
                message_ptr->name = malloc(256 * sizeof(char));
                message_ptr->body = malloc(256 * sizeof(char));
                bzero(message_ptr->name, 256);
                bzero(message_ptr->body, 256);

                strcpy(message_ptr->status, "received");
                receiveMessage(args->client_FD, message_ptr->name);
                fprintf(stderr, "%s", message_ptr->name);
                receiveMessage(args->client_FD, message_ptr->body);
                add_to_pipe(args->outgoing_pipe, message_ptr);


                sendMessage(args->client_FD, "#close");
                return;
        }
        else if(strcmp(buffer, "ADD_SENT") == 0)
        {

                sms_message *message_ptr = malloc(sizeof(sms_message));

                message_ptr->status = malloc(256 * sizeof(char));
                message_ptr->name = malloc(256 * sizeof(char));
                message_ptr->body = malloc(256 * sizeof(char));
                bzero(message_ptr->name, 256);
                bzero(message_ptr->body, 256);

                strcpy(message_ptr->status, "sent");
                receiveMessage(args->client_FD, message_ptr->name);
                fprintf(stderr, "%s", message_ptr->name);
                receiveMessage(args->client_FD, message_ptr->body);
                add_to_pipe(args->outgoing_pipe, message_ptr);


                sendMessage(args->client_FD, "#close");
                return;
        }

        else if(strcmp(buffer, "ADD_SENDING") == 0)
        {
                sms_message *message_ptr = malloc(sizeof(sms_message));

                message_ptr->status = malloc(256 * sizeof(char));
                message_ptr->name = malloc(256 * sizeof(char));
                message_ptr->body = malloc(256 * sizeof(char));
                bzero(message_ptr->name, 256);
                bzero(message_ptr->body, 256);

                strcpy(message_ptr->status, "sending");
                receiveMessage(args->client_FD, message_ptr->name);
                fprintf(stderr, "%s", message_ptr->name);
                receiveMessage(args->client_FD, message_ptr->body);
                add_to_pipe(args->outgoing_pipe, message_ptr);


                sendMessage(args->client_FD, "#close");
                return;
        }
	
	else if(strcmp(buffer, "ADD_CONTACT") == 0)
	{
		char name[256];
		bzero(name, 256); 
		receiveMessage(args->client_FD, name); 
		
		pthread_mutex_lock(args->db_lock);
		{
			char query[1000];
			bzero(query, 1000); 
			strcat(query, "INSERT INTO contacts VALUES(\"");
			strcat(query, name);
			strcat(query, "\", \""); 
			strcat(query, "0000-00-00T00:00:00:000\", \"");
			strcat(query, "0000-00-00T00:00:00:000\");");
			run_query(query);  
		}
		pthread_mutex_unlock(args->db_lock); 
                sendMessage(args->client_FD, "#close"); 
                return;
	}
	
}

