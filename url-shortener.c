#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "functions.h"

// Holds user input. Might be Deprecated due to thrad creation.
struct ARGS{
	int threads;
	char* url;
	char hashed_url[27];
	mongoc_client_t *client;
};

struct thread_arg_t{
	int thread_id;
};

// Global varables.

/* Muxes */
pthread_mutex_t request_mutex;
pthread_mutexattr_t mta;

/* Condition Variable */
pthread_cond_t got_request = PTHREAD_COND_INITIALIZER;

/**
 * Parse user terminal input; options: amount of threads, insert url and queue database.
 * @params argc { int } how many arguements where given.
 * 	   argv { char* array} holds values as strings
 * 	   args { struct pointer of type ARGS} container to hold all possible input values.
 */
void arg_parser(int argc, char* argv[],struct ARGS *args){

	int opt;

	while( ( opt = getopt(argc,argv, ":i:u:q:") ) != -1){
		switch(opt){
			case 'i':
				printf("Number of Threads: %d\n",atoi(optarg));
				args->threads = atoi(optarg);
				break;
			case 'u':
				printf("URL to be hashed: %s\n",optarg);
				args->url = optarg;
				break;
			case 'q':
				printf("Hashed URL: %s\n",optarg);
				strcpy(args->hashed_url,optarg);
				break;
			case '?':
				printf("Unknown option: %c\n",opt);
				break;
		}
	}

	// optind is for extra arguments
	// Which are not parsed
	for(; optind < argc; optind++)
		printf("Extra Arguments: %s\n", argv[optind]);

}
/**
 *  Handles request given to thread.
 *  @params { struct thread_arg_t } holds information about the thread
 */
void *handle_request(void *thread_arg){
	
	pthread_mutex_lock(&request_mutex);
	struct thread_arg_t args = *(struct thread_arg_t*) thread_arg;
	uint8_t id = args.thread_id;
	printf( "Thread %d online...\n",id);
	pthread_mutex_unlock(&request_mutex);

	sleep(1);
	/*
	// Url hash converter:
	if(false){
	b62_converter(arguements.url,arguements.hashed_url);
	printf("URL: %s\nHash: %s\n",arguements.url,arguements.hashed_url);
	}

	// Insertion:
	if(false)
	if( !insert_DB(arguements.client, arguements.hashed_url, arguements.url) ){
		fprintf(stderr, "Failed Insertion\n");
	}
	
	// Query:
	if(false)
	if( ! query_DB(arguements.client,arguements.hashed_url) ){
		fprintf(stderr, "Query Failed\n");
	}
	*/

}
int main(int argc, char* argv[]){
	
	// Variables:
	struct ARGS arguements;

	arg_parser(argc, argv, &arguements);

	// INIT: libmongoc's internals.
	mongoc_init();

	// Create new client instance:

	// Init: Threads & Mutxes.
	pthread_mutexattr_init(&mta);
	pthread_mutexattr_settype(&mta, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&request_mutex, &mta);

	pthread_t threads [ arguements.threads ];
	struct thread_arg_t thread_args[ arguements.threads ];

	// Create user inputed amount of threads.
	for( int i = 0; i < arguements.threads; i++){
		// Intialize arguements for thread.
		thread_args[i].thread_id = i;
		if(pthread_create( &threads[i], NULL, handle_request,&thread_args[i]) ){
			fprintf(stderr, "Error creating Thread\n");
		}
	}

	/* Server: TODO Getting ready to set up*/
	sleep(5);
	
	// Clean up libmongoc.
	mongoc_client_destroy (arguements.client);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}


