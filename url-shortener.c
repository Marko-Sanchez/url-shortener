#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "functions.h"

// Holds user input.
struct ARGS{
	uint8_t threads;
	char* url;
	char hashed_url[27];
	mongoc_client_t *client;
};

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
				args->threads = opt;
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
int main(int argc, char* argv[]){
	
	// Variables:
	struct ARGS arguements;

	arg_parser(argc, argv, &arguements);

	// INIT: libmongoc's internals.
	mongoc_init();

	// Create new client instance:

	// Url hash converter:
	if(true){
	b62_converter(arguements.url,arguements.hashed_url);
	printf("URL: %s\nHash: %s\n",arguements.url,arguements.hashed_url);
	}

	// Insertion:
	if(true)
	if( !insert_DB(arguements.client, arguements.hashed_url, arguements.url) ){
		fprintf(stderr, "Failed Insertion\n");
	}
	
	// Query:
	if(true)
	if( ! query_DB(arguements.client,arguements.hashed_url) ){
		fprintf(stderr, "Query Failed\n");
	}

	// Clean up libmongoc.
	mongoc_client_destroy (arguements.client);
	mongoc_cleanup ();

	return EXIT_SUCCESS;
	
}


