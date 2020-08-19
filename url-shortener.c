#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

#include "functions.h"

#define MAX_URL_LENGTH 1000
#define MAX_HASH_LENGTH 250
#define NUM_THREADS 4
#define API_KEY " YOUR API KEY"

// Container: For thread processing.
struct request{
	char req[3];
	char str[MAX_URL_LENGTH];
	char hashed_str[MAX_HASH_LENGTH];
	bool insert;
	struct request *next;
}*head = NULL, *tail = NULL;

// Thread specific information.
struct thread_arg_t{
	mongoc_client_pool_t* pool;
	int thread_id;
};

/* Muxes */
pthread_mutex_t request_mutex;
pthread_mutexattr_t mutex_atrribute;
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

// Alerts threads to shut down.
bool in_shutdown = false;
int request_to_process = 0;

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
	mongoc_client_pool_t *pool = args.pool;
	pthread_mutex_unlock(&request_mutex);

	// Handles request once alerted by main().
	struct request *request_to_handle;
	mongoc_client_t *client;
	while(true){
		pthread_mutex_lock(&request_mutex);

		// If broadcast is called, while thread is busy.
		if( !in_shutdown){
			
			if( !(request_to_process >  0) )
			// Wait for alert from main thread.
			pthread_cond_wait(&got_request, &request_mutex);

			// Handle request.
			if(head != NULL){
			request_to_handle = head;
			head = head -> next;
			}else{
				pthread_mutex_unlock(&request_mutex);
				continue;
			}
			
		}
		
		if(in_shutdown){
			// Shutting down thread..
			pthread_mutex_unlock(&request_mutex);
			break;
		}

		// Free mutex, since we no longer are in critical area.
		pthread_mutex_unlock(&request_mutex);

		
		printf("Thread %d is handling: %s\n",ID, request_to_handle-> hashed_str);
		// Thread-Safe call for client.
		client = mongoc_client_pool_pop(pool);
		if( request_to_handle -> insert ){
			// Url hash converter:
			b62_converter( request_to_handle->str,
					request_to_handle-> hashed_str);

			// Insertion:
			if( !insert_DB(client
					,request_to_handle->hashed_str
					,request_to_handle -> str) )
				fprintf(stderr, "Failed Insertion\n");
		}
		else{ 
			// Query:
			if( ! query_DB(client,request_to_handle ->hashed_str) ){
				fprintf(stderr, "Query Failed\n");
			}

		}

		// Release client back to pool.
		mongoc_client_pool_push(pool,client);
		// Free pointer allocated with malloc.
		free(request_to_handle);

		// Thread finished procesing,thus decrement.
		pthread_mutex_lock(&request_mutex);
		--request_to_process;
		pthread_mutex_unlock(&request_mutex);
	}

	// Closing thread to free memory allocated.
	printf( "Closing Thread %d ...\n",ID);

	return NULL;
	
}

int main(){
	const char *uri_string = API_KEY;
	mongoc_uri_t *uri;
	mongoc_client_pool_t *pool;
	bson_error_t error;
	void *ret;

	// INIT: libmongoc's internals.
	mongoc_init();

	uri = mongoc_uri_new_with_error(uri_string, & error);
	if(!uri){
		fprintf(stderr,	"failed to parse URI..\nerror message:  %s\n",
				error.message);
		return EXIT_FAILURE;
	}

	pool = mongoc_client_pool_new(uri);

	// Init: Threads & Mutxes.
	pthread_mutexattr_init(&mutex_atrribute);
	pthread_mutexattr_settype(&mutex_atrribute, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&request_mutex, &mutex_atrribute);

	pthread_t threads [ NUM_THREADS ];
	struct thread_arg_t thread_args[ NUM_THREADS ];

	// Create user inputed amount of threads.
	for( unsigned int i = 0; i < NUM_THREADS; ++i){
		// Intialize arguements for thread.
		thread_args[i].thread_id = i;
		thread_args[i].pool = pool;
		if(pthread_create( &threads[i], NULL, handle_request,&thread_args[i]) ){
			fprintf(stderr, "Error creating Thread\n");
		}
	}

	

	char* line = NULL;
	char* delim = " ";
	
	// Reads user input and passes workload to worker threads.
	while( true ){
		
		// Ask user for input and retrieve line.
		size_t len = 0;
		if( getline(&line, &len, stdin) == -1){
			printf("Error message: %s\n",strerror(errno) );
			free(line);
			continue;
		}

		// Remove new-line char.
		char *newline = strchr(line,'\n');
		if( newline )
			*newline = 0;
		
		if( line != NULL && strlen(line) != 0){

			char* current_location;
			char* req = strtok_r(line , delim, &current_location );
			char* value = strtok_r(NULL, delim, &current_location); 

			// Check for buffer overflow
			if(strlen(req)  > 3 || value == NULL ||  strlen(value) == 0 ){
				free(line);
				fprintf(stderr,"\nREQUEST BUFFER OVERFLOW OR INVALID INPUT URL\nTry Again\n\n");
				continue;
			}
			// Freed in thread.
			struct request *new_request;
			new_request = malloc(sizeof *new_request);

			strcpy(new_request->req, req );
			strcpy(new_request->str, value);

			// returns false if invalid command is passed.
			if( arg_parser(new_request) ){

				// Add to pointers: CRITICAL AREA
				pthread_mutex_lock(&request_mutex);
				if(head == NULL){
					head = new_request;
					tail = new_request;
				}else{
					tail->next = new_request;
					tail = tail->next;
				}
				// Thread wil process request.
				++request_to_process;

				pthread_cond_signal(&got_request);
				pthread_mutex_unlock(&request_mutex);
			}

			// Free char* malloc'd by getline.
			free(line);
		}else{
			// Wait till threads finish processing.
			while( request_to_process != 0){
				sleep(1);
			}

			// User pressed enter thus, they want to exit. 
			printf("\n\nExiting program...\n");
			free(line);
			break;
		}

	}

	pthread_mutex_lock( &request_mutex);
	in_shutdown = true;
	pthread_mutex_unlock( &request_mutex);

	// Signal every thread to end.
	pthread_cond_broadcast( &got_request);
	for( unsigned int i = 0; i < NUM_THREADS; ++i){
		pthread_join( threads[i],&ret);
	}



	// Clean up libmongoc.
	mongoc_client_pool_destroy(pool);
	mongoc_uri_destroy(uri);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}


