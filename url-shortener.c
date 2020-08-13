#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "functions.h"

#define MAX_URL_LENGTH 1000
#define MAX_HASH_LENGTH 250
// Holds user input. Might be Deprecated due to thrad creation.
struct request{
	char req[3];
	char str[MAX_URL_LENGTH];
	char hashed_str[MAX_HASH_LENGTH];
	bool insert;
	struct request *next;
}*head, *tail;

struct thread_arg_t{
	mongoc_client_t* client;
	int thread_id;
};

/* Muxes */
pthread_mutex_t request_mutex;
pthread_mutexattr_t mutex_atrribute;
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

/**
 * Parse user terminal input; options: insert url and queue database.
 * @params { struct request *} holds user input: request comman, url/ key. 
 */
bool arg_parser(struct request *request_to_handle ){
	
	if( strcmp( request_to_handle -> req, "-i") == 0 ){
		request_to_handle -> insert = true;
	}else if( strcmp( request_to_handle -> req, "-q") == 0 ){
		request_to_handle -> insert = false;
		strcpy( request_to_handle -> hashed_str, request_to_handle->str);
		// Set str to 0 / empty it.
		memset(request_to_handle ->str,0,MAX_URL_LENGTH);
	}else{
		fprintf(stderr, "Unkown command try again...\n\n");
		return false;
	}
	return true;
}
/**
 *  Handles request given to thread.
 *  @params { struct thread_arg_t } holds information about the thread
 */
void *handle_request(void *thread_arg){
	
	// Starting up thread.
	pthread_mutex_lock(&request_mutex);
	struct thread_arg_t args = *(struct thread_arg_t*) thread_arg;
	uint8_t ID = args.thread_id;
	mongoc_client_t *client = args.client;
	printf( "Thread %d online...\n",ID);
	pthread_mutex_unlock(&request_mutex);

	// Handles request once alerted by main().
	struct request *request_to_handle;
	while(true){
		pthread_mutex_lock(&request_mutex);
		// Wait for alert from main thread.
		pthread_cond_wait(&got_request, &request_mutex);

		// Handle request.
		request_to_handle = head;
		head = head -> next;
		// Free mutex, since we no longer are in critical area.
		pthread_mutex_unlock(&request_mutex);

		
		if( request_to_handle -> insert ){
		printf("Thread %d is handling request: %s : %s \n",ID, request_to_handle->req,request_to_handle->str );
			// Url hash converter:
			b62_converter( request_to_handle->str,
					request_to_handle-> hashed_str);
			printf("URL: %s\nHash: %s\n",request_to_handle-> str
					,request_to_handle -> hashed_str);

			// Insertion:
			if( !insert_DB(client
					,request_to_handle->hashed_str
					,request_to_handle -> str) )
				fprintf(stderr, "Failed Insertion\n");
		}
		else{ 
		printf("Thread %d is handling request: %s : %s \n",ID, request_to_handle->req,request_to_handle->hashed_str);
			// Query:
			if( ! query_DB(client,request_to_handle ->hashed_str) ){
				fprintf(stderr, "Query Failed\n");
			}

		}
		// Free pointer allocated with calloc.
		free(request_to_handle);
	}
	
}

int main(){

	// INIT: libmongoc's internals.
	mongoc_init();

	// Create new client instance:

	// Init: Threads & Mutxes.
	pthread_mutexattr_init(&mutex_atrribute);
	pthread_mutexattr_settype(&mutex_atrribute, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&request_mutex, &mutex_atrribute);

	pthread_t threads [ 4 ];
	struct thread_arg_t thread_args[ 4 ];

	// Create user inputed amount of threads.
	for( int i = 0; i < 4 ; i++){
		// Intialize arguements for thread.
		thread_args[i].thread_id = i;
		thread_args[i].client = client;
		if(pthread_create( &threads[i], NULL, handle_request,&thread_args[i]) ){
			fprintf(stderr, "Error creating Thread\n");
		}
	}

	/* Server: TODO Getting ready to set up*/
	
	char* line = NULL;
	size_t len = 0;
	char* delim = " ";
	while( true ){
		
		// Read from stdin.
		// Get the long string parse it by space.
		if( getline(&line, &len, stdin) == -1){
			printf("Error message: %s\n",strerror(errno) );
			continue;
		}

		// Remove new-lin char.
		char *newline = strchr(line,'\n');
		if( newline )
			*newline = 0;
		
		if( line != NULL && --len != 0 ){
			
			struct request *new_request = (struct request*) calloc(1,sizeof(struct request) );

			char* current_location;
			char* value = strtok_r(line , delim, &current_location );
			strcpy(new_request->req, value );
			strcpy(new_request->str,strtok_r(NULL, delim, &current_location) );

			// returns false if invalid command is passed.
			if( arg_parser(new_request) ){
			printf("Values after arg_parser: %s:%s:%d\n",new_request -> req, new_request->hashed_str,new_request->insert);
			// Add to pointers: CRITICAL AREA
			pthread_mutex_lock(&request_mutex);
				if(head == NULL){
					head = new_request;
					tail = new_request;
				}else{
					tail -> next = new_request;
					tail = new_request;
				}
			pthread_cond_signal(&got_request);
			pthread_mutex_unlock(&request_mutex);
			}// arg_parser()

		}else{
		// User pressed enter thus, they want to exit. 
			free(line);
			break;
		}
		free(line);

	}

	// Clean up libmongoc.
	mongoc_client_destroy (client);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}


